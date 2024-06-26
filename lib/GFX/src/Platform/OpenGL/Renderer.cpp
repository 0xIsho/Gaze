#include "GFX/Platform/OpenGL/Renderer.hpp"

#include "GFX/Platform/OpenGL/Objects/Framebuffer.hpp"
#include "GFX/Platform/OpenGL/Objects/IndexBuffer.hpp"
#include "GFX/Platform/OpenGL/Objects/Object.hpp"
#include "GFX/Platform/OpenGL/Objects/Shader.hpp"
#include "GFX/Platform/OpenGL/Objects/VertexBuffer.hpp"
#include "GFX/Platform/OpenGL/Objects/VertexArray.hpp"

#include "GFX/Light.hpp"

#include "Log/Logger.hpp"

#include "glad/gl.h"

#include <GLFW/glfw3.h>

#include <format>
#include <type_traits>
#include <unordered_map>

namespace Gaze::GFX::Platform::OpenGL {
	static auto EnableDebugOutput()
	{
		int flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if ((flags & GL_CONTEXT_FLAG_DEBUG_BIT) == 0) { // Not a debug context
			return;
		}

		// clang-format off
		static const auto sources = std::unordered_map<GLenum, const char*> {
			{ GL_DEBUG_SOURCE_API               , "API"                 },
			{ GL_DEBUG_SOURCE_WINDOW_SYSTEM     , "Application"         },
			{ GL_DEBUG_SOURCE_SHADER_COMPILER   , "Shader Compiler"     },
			{ GL_DEBUG_SOURCE_THIRD_PARTY       , "Third Party"         },
			{ GL_DEBUG_SOURCE_APPLICATION       , "Application"         },
			{ GL_DEBUG_SOURCE_OTHER             , "Other"               },
		};
		static const auto types = std::unordered_map<GLenum, const char*> {
			{ GL_DEBUG_TYPE_ERROR               , "Error"               },
			{ GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR , "Deprecated Behavior" },
			{ GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR  , "Undefined Behavior"  },
			{ GL_DEBUG_TYPE_PORTABILITY         , "Portability"         },
			{ GL_DEBUG_TYPE_PERFORMANCE         , "Performance"         },
			{ GL_DEBUG_TYPE_MARKER              , "Marker"              },
			{ GL_DEBUG_TYPE_PUSH_GROUP          , "Push Group"          },
			{ GL_DEBUG_TYPE_POP_GROUP           , "Pop Group"           },
			{ GL_DEBUG_TYPE_OTHER               , "Other"               },
		};
		/*static const auto severities = std::unordered_map<GLenum, const char*> {
			{ GL_DEBUG_SEVERITY_HIGH            , "High"                },
			{ GL_DEBUG_SEVERITY_MEDIUM          , "Medium"              },
			{ GL_DEBUG_SEVERITY_LOW             , "Low"                 },
			{ GL_DEBUG_SEVERITY_NOTIFICATION    , "Notification"        },
		};*/
		// clang-format on
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

		glDebugMessageCallback(
			[](
				GLenum source,
				GLenum type,
				GLuint /* id */,
				GLenum severity,
				GLsizei /* length */,
				const GLchar* message,
				const void* /* userParam */
			) {
				auto logger = Log::Logger("Renderer");

				static constexpr char fmt[] = "OpenGL Message:\n  - Source: {}\n  - Type: {}\n  - Message: {}";

				switch (severity) {
				case GL_DEBUG_SEVERITY_NOTIFICATION:
					logger.Trace(fmt, sources.at(source), types.at(type), message);
					break;
				case GL_DEBUG_SEVERITY_LOW:
					logger.Warn(fmt, sources.at(source), types.at(type), message);
					break;
				case GL_DEBUG_SEVERITY_MEDIUM:
					logger.Error(fmt, sources.at(source), types.at(type), message);
					break;
				case GL_DEBUG_SEVERITY_HIGH:
					logger.Critical(fmt, sources.at(source), types.at(type), message);
					break;
				}
			},
			nullptr
		);
	}

	static constexpr auto kMaxLights = 8;

	struct BufferSection
	{
		I32 offset;
		I32 size;

		Renderer::PrimitiveMode mode;
		Object::Properties      properties;
		Light                   lights[kMaxLights];
		I32                     nLights;
	};

	struct ScreenQuadVertex
	{
		float x, y, z;
		float u, v;
	};

	struct Renderer::Impl
	{
		Objects::VertexArray                 vertexArray;
		Objects::VertexArray                 screenVA;
		Objects::VertexBuffer                screenVB;
		Objects::IndexBuffer                 screenIB;
		Objects::ShaderProgram               program;
		Objects::ShaderProgram               screenProgram;
		Objects::VertexBuffer                vertexBuf;
		Objects::IndexBuffer                 indexBuf;
		Objects::Framebuffer                 framebuffer;
		std::vector<BufferSection>           vertexBufSects;
		std::vector<BufferSection>::iterator vertexBufSectsCursor;
		std::vector<BufferSection>           indexBufSects;
		std::vector<BufferSection>::iterator indexBufSectsCursor;
		Shared<Camera>                       camera;
		RenderStats                          stats;
		RenderStats                          statsCurrent;
		Log::Logger                          logger;
	};

	static constexpr auto kStaticBufferSize = 8 * 1024 * 1024; // 8 MiB

	Renderer::Renderer(Shared<WM::Window> window) noexcept
		: GFX::Renderer(std::move(window))
		, m_pImpl(nullptr)
	{
		auto* oldCurrentContext = glfwGetCurrentContext();
		glfwMakeContextCurrent(static_cast<GLFWwindow*>(Window().Handle()));

		gladLoadGL(static_cast<GLADloadfunc>(glfwGetProcAddress));

#ifndef NDEBUG
		EnableDebugOutput();
#endif

		const auto* vertexSource = R"(
			#version 330 core
			layout (location = 0) in vec3 a_Position;
			layout (location = 1) in vec3 a_Normal;

			uniform mat4 u_model;
			uniform mat4 u_vp;

			out vec3 normal;
			out vec3 surfacePos;

			void main()
			{
				gl_Position = u_vp * u_model * vec4(a_Position, 1.0);

				normal = a_Normal;
				surfacePos = vec3(u_model * vec4(a_Position, 1.0));
			}
		)";
		const auto* fragmentSource = R"(
			#version 330 core

			struct Material
			{
				vec3 diffuse;
				vec3 specular;
				float shininess;
			};

			struct Light
			{
				vec3 position;
				vec3 diffuse;
				float ambientCoefficient;
				float attenuation;
			};
			const int kMaxLights = 8;

			out vec4 FragColor;

			uniform Material u_Material;
			uniform Light u_Light;
			uniform Light u_Lights[kMaxLights];
			uniform int u_nLights;
			uniform vec3 u_ViewPos;

			in vec3 normal;
			in vec3 surfacePos;

			vec3 ComputeLight(Light light, vec3 normal, vec3 surfacePos, vec3 surfaceToView)
			{
				vec3 surfaceToLight = normalize(light.position - surfacePos);
				float diffuseCoefficient = max(dot(normal, surfaceToLight), 0.0);
				vec3 diffuse = diffuseCoefficient * u_Material.diffuse * light.diffuse;

				vec3 specular = vec3(0);
				if (diffuseCoefficient > 0) {
					float specularCoefficient = pow(max(0.0, dot(surfaceToView, reflect(-surfaceToLight, normal))), u_Material.shininess);
					specular = specularCoefficient * u_Material.specular * light.diffuse;
				}

				vec3 ambient = light.ambientCoefficient * light.diffuse * u_Material.diffuse.rgb;
				float attenuation = 1.0 / (1.0 + light.attenuation * pow(length(light.position - surfacePos), 2));

				return ambient + attenuation * (diffuse + specular);
			}

			void main()
			{
				const vec3 gamma = vec3(1.0 / 2.2);

				vec3 linearColor = vec3(0);
				vec3 surfaceToView = normalize(u_ViewPos - surfacePos);
				for (int i = 0; i < u_nLights; i++) {
					linearColor += ComputeLight(u_Lights[i], normal, surfacePos, surfaceToView);
				}

				FragColor = vec4(pow(linearColor, gamma), 1.0);
			}
		)";

		const auto* screenQuadVertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TextureCoords;

			out vec2 TextureCoords;

			void main()
			{
				gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
				TextureCoords = a_TextureCoords;
			}
		)";

		const auto* screenQuadFragmentSource = R"(
			#version 330 core

			out vec4 FragColor;

			in vec2 TextureCoords;

			uniform sampler2D screenTexture;

			void main()
			{
				FragColor = texture(screenTexture, TextureCoords);
			}
		)";

		auto vShader = Objects::Shader(Objects::Shader::Type::Vertex, vertexSource);
		GAZE_ASSERT(vShader.Compile(), "Failed to compile Vertex shader");
		auto fShader = Objects::Shader(Objects::Shader::Type::Fragment, fragmentSource);
		GAZE_ASSERT(fShader.Compile(), "Failed to compile Fragment shader");

		auto screenVShader = Objects::Shader(Objects::Shader::Type::Vertex, screenQuadVertexSource);
		GAZE_ASSERT(screenVShader.Compile(), "Failed to compile Screen Vertex shader");
		auto screenFShader = Objects::Shader(Objects::Shader::Type::Fragment, screenQuadFragmentSource);
		GAZE_ASSERT(screenFShader.Compile(), "Failed to compile Screen Fragment shader");

		const ScreenQuadVertex screenQuadVertices[4] = {
			{ -1.0F, -1.0F, 0.0F, 0.0F, 0.0F },
			{  1.0F, -1.0F, 0.0F, 1.0F, 0.0F },
			{ -1.0F,  1.0F, 0.0F, 0.0F, 1.0F },
			{  1.0F,  1.0F, 0.0F, 1.0F, 1.0F },
		};
		const U32 screenQuadIndices[6] = {
			0, 1, 2, 3
		};

		m_pImpl = new Impl({
			.vertexArray          = {},
			.screenVA             = {},
			.screenVB             = Objects::VertexBuffer(screenQuadVertices, sizeof(screenQuadVertices), Objects::BufferUsage::StaticDraw),
			.screenIB             = Objects::IndexBuffer(screenQuadIndices, sizeof(screenQuadIndices), Objects::BufferUsage::StaticDraw),
			.program              = { &vShader, &fShader },
			.screenProgram        = { &screenVShader, &screenFShader },
			.vertexBuf            = Objects::VertexBuffer(nullptr, kStaticBufferSize, Objects::BufferUsage::DynamicDraw),
			.indexBuf             = Objects::IndexBuffer(nullptr, kStaticBufferSize, Objects::BufferUsage::DynamicDraw),
			.framebuffer          = { Window().Width(), Window().Height() },
			.vertexBufSects       = {},
			.vertexBufSectsCursor = {},
			.indexBufSects        = {},
			.indexBufSectsCursor  = {},
			.camera               = {
				MakeShared<PerspectiveCamera>(
					glm::radians(75.F),
					F32(Window().Width()) / F32(Window().Height()),
					.1F,
					100.F
				)
			},
			.stats                = {},
			.statsCurrent         = {},
			.logger               = Log::Logger("Renderer")
		});

		GAZE_ASSERT(m_pImpl->program.Link(), "Failed to link shader program");
		GAZE_ASSERT(m_pImpl->screenProgram.Link(), "Failed to link screen shader program");
		m_pImpl->screenProgram.Use();
		m_pImpl->screenProgram.UploadUniform1I("screenTexture", 0);

		m_pImpl->vertexBufSects.resize(kStaticBufferSize / sizeof(BufferSection));
		m_pImpl->vertexBufSectsCursor = m_pImpl->vertexBufSects.begin();
		m_pImpl->indexBufSects.resize(kStaticBufferSize / sizeof(BufferSection));
		m_pImpl->indexBufSectsCursor = m_pImpl->indexBufSects.begin();

		m_pImpl->vertexArray.Bind();
		m_pImpl->vertexArray.SetIndexBuffer(&m_pImpl->indexBuf);
		m_pImpl->vertexArray.SetLayout({
			{
				Objects::VertexArray::Layout::BufferBinding(0),
				Objects::VertexArray::Layout::ComponentCount(3),
				Objects::VertexArray::Layout::DataType::Float,
				Objects::VertexArray::Layout::Normalized(false),
				Objects::VertexArray::Layout::RelativeOffset(offsetof(Vertex, position))
			},
			{
				Objects::VertexArray::Layout::BufferBinding(0),
				Objects::VertexArray::Layout::ComponentCount(3),
				Objects::VertexArray::Layout::DataType::Float,
				Objects::VertexArray::Layout::Normalized(false),
				Objects::VertexArray::Layout::RelativeOffset(offsetof(Vertex, normals))
			},
		});
		m_pImpl->vertexArray.BindVertexBuffer(
			&m_pImpl->vertexBuf,
			Objects::VertexArray::Layout::BufferBinding(0),
			Objects::VertexArray::Offset(0),
			Objects::VertexArray::Stride(sizeof(Vertex))
		);

		m_pImpl->screenVA.Bind();
		m_pImpl->screenVA.SetIndexBuffer(&m_pImpl->screenIB);
		m_pImpl->screenVA.SetLayout({
			{
				Objects::VertexArray::Layout::BufferBinding(0),
				Objects::VertexArray::Layout::ComponentCount(3),
				Objects::VertexArray::Layout::DataType::Float,
				Objects::VertexArray::Layout::Normalized(false),
				Objects::VertexArray::Layout::RelativeOffset(offsetof(ScreenQuadVertex, x))
			},
			{
				Objects::VertexArray::Layout::BufferBinding(0),
				Objects::VertexArray::Layout::ComponentCount(2),
				Objects::VertexArray::Layout::DataType::Float,
				Objects::VertexArray::Layout::Normalized(false),
				Objects::VertexArray::Layout::RelativeOffset(offsetof(ScreenQuadVertex, u))
			}
		});
		m_pImpl->screenVA.BindVertexBuffer(
			&m_pImpl->screenVB,
			Objects::VertexArray::Layout::BufferBinding(0),
			Objects::VertexArray::Offset(0),
			Objects::VertexArray::Stride(sizeof(ScreenQuadVertex))
		);

		{
			const GLubyte* info[] = {
				glGetString(GL_VENDOR),
				glGetString(GL_RENDERER),
				glGetString(GL_VERSION),
				glGetString(GL_SHADING_LANGUAGE_VERSION),
			};

			m_pImpl->logger.Info(
				"OpenGL Info:\n  - Vendor: {}\n  - Renderer: {}\n  - Version: {}\n  - GLSL Version: {}",
				info[0] ? reinterpret_cast<const char*>(info[0]) : "Unknown",
				info[1] ? reinterpret_cast<const char*>(info[1]) : "Unknown",
				info[2] ? reinterpret_cast<const char*>(info[2]) : "Unknown",
				info[3] ? reinterpret_cast<const char*>(info[3]) : "Unknown"
			);
		}

		if (oldCurrentContext) {
			glfwMakeContextCurrent(oldCurrentContext);
		}
	}

	Renderer::~Renderer()
	{
		delete m_pImpl;
	}

	auto Renderer::SetClearColor(F32 r, F32 g, F32 b, F32 a) noexcept -> void
	{
		glClearColor(r, g, b, a);
	}

	auto Renderer::Clear(Buffer buffer) noexcept -> void
	{
		auto bufferBits = 0U;

		if (buffer & Buffer::kColorBuffer) {
			bufferBits |= GL_COLOR_BUFFER_BIT;
		}
		if (buffer & Buffer::kDepthBuffer) {
			bufferBits |= GL_DEPTH_BUFFER_BIT;
		}
		if (buffer & Buffer::kStencilBuffer) {
			bufferBits |= GL_STENCIL_BUFFER_BIT;
		}

		m_pImpl->framebuffer.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Objects::Framebuffer::Unbind();
		glClear(bufferBits);
	}

	auto Renderer::Flush() noexcept -> void
	{
		m_pImpl->framebuffer.Bind();
		m_pImpl->vertexArray.Bind();
		m_pImpl->program.Use();

		const auto vp = m_pImpl->camera->ComputeProjectionMatrix() * m_pImpl->camera->ComputeViewMatrix();

		m_pImpl->program.UploadUniform3FV("u_ViewPos", &(m_pImpl->camera->Position()[0]));
		m_pImpl->program.UploadUniformMatrix4FV("u_vp", &(vp[0][0]));

		for (auto it = m_pImpl->indexBufSects.cbegin(); it != m_pImpl->indexBufSectsCursor; ++it) {
			const auto& sect = *it;
			auto drawMode = GLenum();

			switch (sect.mode) {
			case PrimitiveMode::Points:        drawMode = GL_POINTS;         break;
			case PrimitiveMode::Lines:         drawMode = GL_LINES;          break;
			case PrimitiveMode::LineLoop:      drawMode = GL_LINE_LOOP;      break;
			case PrimitiveMode::LineStrip:     drawMode = GL_LINE_STRIP;     break;
			case PrimitiveMode::Triangles:     drawMode = GL_TRIANGLES;      break;
			case PrimitiveMode::TriangleStrip: drawMode = GL_TRIANGLE_STRIP; break;
			case PrimitiveMode::TriangleFan:   drawMode = GL_TRIANGLE_FAN;   break;
			default:                           drawMode = GL_TRIANGLES;      break;
			}

			for (auto i = 0; const auto& light : sect.lights) {
				const auto uniform = std::format("u_Lights[{}]", i++);

				m_pImpl->program.UploadUniform3FV(std::format("{}.position", uniform), &(light.position[0]));
				m_pImpl->program.UploadUniform3FV(std::format("{}.diffuse", uniform),  &(light.diffuse[0]));
				m_pImpl->program.UploadUniform1F(std::format("{}.ambientCoefficient", uniform), light.ambientCoefficient);
				m_pImpl->program.UploadUniform1F(std::format("{}.attenuation", uniform), light.attenuation);
			}
			m_pImpl->program.UploadUniform1I("u_nLights", sect.nLights);

			m_pImpl->program.UploadUniformMatrix4FV("u_model", &(sect.properties.transform[0][0]));
			m_pImpl->program.UploadUniform3FV("u_Material.diffuse", &(sect.properties.material.diffuse[0]));
			m_pImpl->program.UploadUniform3FV("u_Material.specular", &(sect.properties.material.specular[0]));
			m_pImpl->program.UploadUniform1F("u_Material.shininess", sect.properties.material.shininess);

			const auto idx = std::size_t(std::distance(m_pImpl->indexBufSects.cbegin(), it));
			glDrawElementsBaseVertex(
				drawMode,
				sect.size / Mesh::kIndexSize,
				GL_UNSIGNED_INT,
				reinterpret_cast<void*>(sect.offset),
				m_pImpl->vertexBufSects[idx].offset / Mesh::kVertexSize
			);
			m_pImpl->statsCurrent.nDrawCalls++;
		}
		Objects::Framebuffer::Unbind();
		glDisable(GL_DEPTH_TEST);

		m_pImpl->screenVA.Bind();
		m_pImpl->screenProgram.Use();
		glBindTextureUnit(0, m_pImpl->framebuffer.ColorAttachmentID());
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
		glEnable(GL_DEPTH_TEST);

		m_pImpl->vertexBufSectsCursor = m_pImpl->vertexBufSects.begin();
		m_pImpl->indexBufSectsCursor = m_pImpl->indexBufSects.begin();
	}

	auto Renderer::Render() noexcept -> void
	{
		Flush();
		glfwSwapBuffers(static_cast<GLFWwindow*>(Window().Handle()));

		m_pImpl->stats = m_pImpl->statsCurrent;
		m_pImpl->statsCurrent = RenderStats();
	}

	auto Renderer::MakeContextCurrent() noexcept -> void
	{
		glfwMakeContextCurrent(static_cast<GLFWwindow*>(Window().Handle()));
	}

	auto Renderer::Stats() noexcept -> RenderStats
	{
		return m_pImpl->stats;
	}

	auto Renderer::SetViewport(I32 x, I32 y, I32 width, I32 height) noexcept -> void
	{
		glViewport(x, y, width, height);
	}

	auto Renderer::SetCamera(Shared<Camera> camera) noexcept -> void
	{
		m_pImpl->camera = std::move(camera);
	}

	auto Renderer::DrawMesh(const Mesh& mesh, PrimitiveMode mode) -> void
	{
		const auto lights = Light {
			.position           = { 0.F, 0.F, 0.F },
			.diffuse            = { 1.F, 1.F, 1.F },
			.ambientCoefficient = 1.F,
			.attenuation        = 1.F
		};

		DrawMesh(mesh, &lights, 1, mode);
	}

	auto Renderer::DrawMesh(const Mesh& mesh, const Light lights[], I32 nLights, PrimitiveMode mode) -> void
	{
		auto primitives = std::vector<Geometry::Primitive>();
		for (const auto& prim : mesh.Primitives()) {
			auto vertices = std::vector<Geometry::Vertex>();

			for (const auto& vert : prim.vertices) {
				vertices.emplace_back(Geometry::Vertex {
					.x = vert.position.x,
					.y = vert.position.y,
					.z = vert.position.z,
					.nx = vert.normals.x,
					.ny = vert.normals.y,
					.nz = vert.normals.z
				});
			}
			primitives.emplace_back(Geometry::Primitive { std::move(vertices), prim.indices });
		}

		SubmitObject(
			Object(
				Geometry::Mesh(
					std::move(primitives)),
					{ mesh.Transform(), mesh.Material() }
			),
			lights,
			nLights,
			mode
		);
	}

	auto Renderer::SubmitObject(const Object& object, PrimitiveMode mode) -> void
	{
		const auto lights = Light {
			.position           = { 0.F, 0.F, 0.F },
			.diffuse            = { 1.F, 1.F, 1.F },
			.ambientCoefficient = 1.F,
			.attenuation        = 1.F
		};

		SubmitObject(object, &lights, 1, mode);
	}

	auto Renderer::SubmitObject(const Object& object, const Light lights[], I32 nLights, PrimitiveMode mode) -> void
	{
		GAZE_ASSERT(lights != nullptr, "Missing lights");
		GAZE_ASSERT(nLights > 0, "Must provide at least 1 light source");
		GAZE_ASSERT(nLights <= 8, "Each Mesh may have a maximum of 8 light sources influencing it");

		const auto& mesh = object.Mesh();

		{
			auto offset = 0;

			for (auto it = mesh.Primitives().cbegin(); it != mesh.Primitives().cend(); ++it) {
				const auto& prim = *it;

				if (m_pImpl->vertexBufSectsCursor != m_pImpl->vertexBufSects.begin()) {
					const auto& last = m_pImpl->vertexBufSectsCursor - 1;
					offset = last->offset + last->size;
				}

				if (offset > m_pImpl->vertexBufSects.size()) {
					Flush();
					offset = 0;
				}

				auto sect = BufferSection{
					offset,
					I32(prim.vertices.size() * mesh.kVertexSize),
					mode,
					object.GetProperties(),
					{},
					nLights
				};
				static_assert(std::is_standard_layout_v<Light> && std::is_trivially_copyable_v<Light>);
				memcpy(sect.lights, lights, size_t(nLights) * sizeof(Light));

				*m_pImpl->vertexBufSectsCursor = sect;
				m_pImpl->vertexBufSectsCursor++;

				m_pImpl->vertexBuf.Upload(
					prim.vertices.data(),
					I32(prim.vertices.size() * mesh.kVertexSize),
					offset
				);
			}
		}

		{
			auto offset = 0;

			for (auto it = mesh.Primitives().cbegin(); it != mesh.Primitives().cend(); ++it) {
				const auto& prim = *it;

				if (m_pImpl->indexBufSectsCursor != m_pImpl->indexBufSects.begin()) {
					const auto& last = m_pImpl->indexBufSectsCursor - 1;
					offset = last->offset + last->size;
				}

				if (offset > m_pImpl->vertexBufSects.size()) {
					Flush();
					offset = 0;
				}

				auto sect = BufferSection{
					offset,
					I32(prim.indices.size() * mesh.kIndexSize),
					mode,
					object.GetProperties(),
					{},
					nLights
				};
				static_assert(std::is_standard_layout_v<Light> && std::is_trivially_copyable_v<Light>);
				memcpy(sect.lights, lights, size_t(nLights) * sizeof(Light));

				*m_pImpl->indexBufSectsCursor = sect;
				m_pImpl->indexBufSectsCursor++;

				m_pImpl->indexBuf.Upload(
					prim.indices.data(),
					I32(prim.indices.size() * Mesh::kIndexSize),
					offset
				);
			}
		}
	}
}

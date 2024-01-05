#include "GFX/Platform/OpenGL/Renderer.hpp"

#include "GFX/Platform/OpenGL/Objects/IndexBuffer.hpp"
#include "GFX/Platform/OpenGL/Objects/Object.hpp"
#include "GFX/Platform/OpenGL/Objects/Shader.hpp"
#include "GFX/Platform/OpenGL/Objects/VertexBuffer.hpp"
#include "GFX/Platform/OpenGL/Objects/VertexArray.hpp"

#include "GFX/Light.hpp"

#include "glad/gl.h"

#include <GLFW/glfw3.h>

#include <cstdio>
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
		static const auto severities = std::unordered_map<GLenum, const char*> {
			{ GL_DEBUG_SEVERITY_HIGH            , "High"                },
			{ GL_DEBUG_SEVERITY_MEDIUM          , "Medium"              },
			{ GL_DEBUG_SEVERITY_LOW             , "Low"                 },
			{ GL_DEBUG_SEVERITY_NOTIFICATION    , "Notification"        },
		};
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
				fprintf(stderr,
					"OpenGL Message {\n  Source: %s\n  Type: %s\n  Severity: %s\n  Message: %s\n}\n",
					sources.at(source),
					types.at(type),
					severities.at(severity),
					message);
			},
			nullptr
		);
	}

	struct BufferSection
	{
		I32 offset;
		I32 size;
		Renderer::PrimitiveMode mode;

		// TODO: These are the same properies in Mesh. The duplication should be fixed somehow
		glm::mat4 transform;
		Material material;
	};

	struct Renderer::Impl
	{
		Objects::VertexArray vertexArray;
		Objects::ShaderProgram program;
		Objects::VertexBuffer vertexBuf;
		Objects::IndexBuffer indexBuf;
		std::vector<BufferSection> vertexBufSects;
		std::vector<BufferSection> indexBufSects;
		RenderStats stats;
		RenderStats statsCurrent;
		Mem::Shared<Camera> camera;
		glm::mat4 projection;
	};

	static constexpr auto kStaticBufferSize = 8 * 1024 * 1024; // 8 MiB

	Renderer::Renderer(Mem::Shared<WM::Window> window) noexcept
		: GFX::Renderer(std::move(window))
		, m_pImpl(nullptr)
	{
		auto* oldCurrentContext = glfwGetCurrentContext();
		glfwMakeContextCurrent(static_cast<GLFWwindow*>(Window().Handle()));

		gladLoadGL(static_cast<GLADloadfunc>(glfwGetProcAddress));

		glEnable(GL_DEPTH_TEST);

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
				vec3 specular;
				float ambientCoefficient;
				float attenuation;
			};

			out vec4 FragColor;

			uniform Material u_Material;
			uniform Light u_Light;
			uniform vec3 u_ViewPos;

			in vec3 normal;
			in vec3 surfacePos;

			void main()
			{
				vec3 lightDir = normalize(u_Light.position - surfacePos);
				float diffuseCoefficient = max(dot(normal, lightDir), 0.0);
				vec3 diffuse = diffuseCoefficient * u_Material.diffuse * u_Light.diffuse;

				vec3 specular = vec3(0);
				if (diffuseCoefficient > 0) {
					vec3 surfaceToView = normalize(u_ViewPos - surfacePos);
					float specularCoefficient = pow(max(0.0, dot(surfaceToView, reflect(-lightDir, normal))), u_Material.shininess);
					specular = specularCoefficient * u_Material.specular * u_Light.specular;
				}

				vec3 ambient = u_Light.ambientCoefficient * u_Light.diffuse * u_Material.diffuse.rgb;
				float attenuation = 1.0 / (1.0 + u_Light.attenuation * pow(length(u_Light.position - surfacePos), 2));

				const vec3 gamma = vec3(1.0 / 2.2);
				FragColor = vec4(pow(ambient + attenuation * (diffuse + specular), gamma), 1.0);
			}
		)";

		auto vShader = Objects::Shader(Objects::Shader::Type::Vertex, vertexSource);
		if (!vShader.Compile()) {
			return;
		}
		auto fShader = Objects::Shader(Objects::Shader::Type::Fragment, fragmentSource);
		if (!fShader.Compile()) {
			return;
		}

		m_pImpl= new Impl({
			{},
			{ &vShader, &fShader },
			Objects::VertexBuffer(nullptr, kStaticBufferSize, Objects::BufferUsage::DynamicDraw),
			Objects::IndexBuffer(nullptr, kStaticBufferSize, Objects::BufferUsage::DynamicDraw),
			{},
			{},
			{ 0 },
			{ 0 },
			{ Mem::MakeShared<Camera>() },
			{ glm::mat4(1.0F) }
		});

		if (!m_pImpl->program.Link()) {
			return;
		}

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

		glClear(bufferBits);
	}

	auto Renderer::Flush() noexcept -> void
	{
		m_pImpl->vertexArray.Bind();
		m_pImpl->program.Use();

		const auto view = m_pImpl->camera->ComputeViewMatrix();
		const auto vp = m_pImpl->projection * view;

		const auto light = Light {
			.position = { 1.0F, 1.0F, -1.0F },
			.diffuse = { .5F, .5F, .5F },
			.specular = { 1.0F, 1.0F, 1.0F },
			.ambientCoefficient = .05F,
			.attenuation = .2F
		};

		m_pImpl->program.UploadUniform3FV("u_ViewPos", &(m_pImpl->camera->Position()[0]));
		m_pImpl->program.UploadUniformMatrix4FV("u_vp", &(vp[0][0]));
		m_pImpl->program.UploadUniform3FV("u_Light.position", &(light.position[0]));
		m_pImpl->program.UploadUniform3FV("u_Light.diffuse",  &(light.diffuse[0]));
		m_pImpl->program.UploadUniform3FV("u_Light.specular", &(light.specular[0]));
		m_pImpl->program.UploadUniform1F("u_Light.ambientCoefficient", light.ambientCoefficient);
		m_pImpl->program.UploadUniform1F("u_Light.attenuation", light.attenuation);

		for (auto i = 0UL; i < m_pImpl->indexBufSects.size(); i++) {
			const auto& sect = m_pImpl->indexBufSects[i];
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

			m_pImpl->program.UploadUniformMatrix4FV("u_model", &(sect.transform[0][0]));
			m_pImpl->program.UploadUniform3FV("u_Material.diffuse", &(sect.material.diffuse[0]));
			m_pImpl->program.UploadUniform3FV("u_Material.specular", &(sect.material.specular[0]));
			m_pImpl->program.UploadUniform1F("u_Material.shininess", sect.material.shininess);

			glDrawElementsBaseVertex(
				drawMode,
				sect.size / Mesh::kIndexSize,
				GL_UNSIGNED_INT,
				reinterpret_cast<void*>(sect.offset),
				m_pImpl->vertexBufSects[i].offset / Mesh::kVertexSize
			);
			m_pImpl->statsCurrent.nDrawCalls++;
		}

		m_pImpl->vertexBufSects.clear();
		m_pImpl->indexBufSects.clear();
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

	auto Renderer::SetProjection(glm::mat4 projection) noexcept -> void
	{
		m_pImpl->projection = std::move(projection);
	}

	auto Renderer::SetCamera(Mem::Shared<Camera> camera) noexcept -> void
	{
		m_pImpl->camera = std::move(camera);
	}

	auto Renderer::DrawMesh(const Mesh& mesh, PrimitiveMode mode) -> void
	{
		{
			auto offset = 0;

			if (m_pImpl->vertexBufSects.size() > 0) {
				offset = m_pImpl->vertexBufSects.back().offset + m_pImpl->vertexBufSects.back().size;
			}

			m_pImpl->vertexBufSects.emplace_back(
				BufferSection{
					offset,
					I32(mesh.Vertices().size() * mesh.kVertexSize),
					mode,
					mesh.Transform(),
					mesh.Material()
				}
			);

			m_pImpl->vertexBuf.Upload(
				mesh.Vertices().data(),
				I32(mesh.Vertices().size() * mesh.kVertexSize),
				offset
			);
		}

		{
			auto offset = 0;

			if (m_pImpl->indexBufSects.size() > 0) {
				offset = m_pImpl->indexBufSects.back().offset + m_pImpl->indexBufSects.back().size;
			}

			m_pImpl->indexBufSects.emplace_back(
				BufferSection{
					offset,
					I32(mesh.Indices().size() * mesh.kIndexSize),
					mode,
					mesh.Transform(),
					mesh.Material()
				}
			);

			m_pImpl->indexBuf.Upload(
				mesh.Indices().data(),
				I32(mesh.Indices().size() * Mesh::kIndexSize),
				offset
			);
		}
	}
}

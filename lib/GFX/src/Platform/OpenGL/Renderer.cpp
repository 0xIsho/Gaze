#include "GFX/Platform/OpenGL/Renderer.hpp"

#include "GFX/Platform/OpenGL/Objects/IndexBuffer.hpp"
#include "GFX/Platform/OpenGL/Objects/Object.hpp"
#include "GFX/Platform/OpenGL/Objects/Shader.hpp"
#include "GFX/Platform/OpenGL/Objects/VertexBuffer.hpp"

#include "GFX/Light.hpp"

#include "glad/gl.h"

#include <GLFW/glfw3.h>

#include <cstdio>

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
		GLID vaoID;
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

	Renderer::Renderer(Mem::Shared<WM::Window> window)
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
			layout (location = 0) in vec3 aPos;
			layout (location = 1) in vec3 a_Normal;

			uniform mat4 u_model;
			uniform mat4 u_vp;

			out vec3 normal;
			out vec3 surfacePos;

			void main()
			{
				gl_Position = u_vp * u_model * vec4(aPos.x, aPos.y, aPos.z, 1.0);

				normal = a_Normal;
				surfacePos = vec3(u_model * vec4(aPos, 1.0));
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
				float attenuation;
			};

			out vec4 FragColor;

			uniform Material u_Material;
			uniform Light u_Light;
			uniform vec3 u_Ambient;
			uniform vec3 u_ViewPos;

			in vec3 normal;
			in vec3 surfacePos;

			void main()
			{
				vec3 lightDir = normalize(u_Light.position - surfacePos);
				float diffuseCoefficient = max(dot(normal, lightDir), 0.0);
				vec3 diffuse = diffuseCoefficient * u_Material.diffuse * u_Light.diffuse;
				vec3 surfaceToView = normalize(u_ViewPos - surfacePos);

				vec3 specular = vec3(0);
				if (diffuseCoefficient > 0) {
					vec3 reflectDir = reflect(-lightDir, normal);
					specular = u_Material.specular * pow(max(dot(surfaceToView, reflectDir), 0.0), u_Material.shininess) * u_Light.specular;
				}

				const vec3 gamma = vec3(1.0 / 2.2);
				float attenuation = 1.0 / (1.0 + u_Light.attenuation * pow(length(u_Light.position - surfacePos), 2));
				FragColor = vec4(pow(u_Ambient + diffuse + specular, gamma), 1.0);
			}
		)";

		auto vShader = Objects::Shader(Objects::Shader::Type::Vertex, vertexSource);
		if (!vShader.Compile()) {
			const auto log = vShader.RetrieveErrorLog(512);
			fprintf(stderr, "Error compiling vertex shader: %s\n", log.c_str());
			return;
		}
		auto fShader = Objects::Shader(Objects::Shader::Type::Fragment, fragmentSource);
		if (!fShader.Compile()) {
			const auto log = fShader.RetrieveErrorLog(512);
			fprintf(stderr, "Error compiling fragment shader: %s\n", log.c_str());
			return;
		}

		m_pImpl= new Impl({
			[] { auto vao = GLID(0); glGenVertexArrays(1, &vao); return vao; }(),
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
			const auto log = fShader.RetrieveErrorLog(512);
			fprintf(stderr, "Error linking shader program: %s\n", log.c_str());
			return;
		}

		auto oldVAO = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &oldVAO);
		glBindVertexArray(m_pImpl->vaoID);
		m_pImpl->vertexBuf.Bind();
		m_pImpl->indexBuf.Bind();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(0));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindVertexArray(GLID(oldVAO));

		if (oldCurrentContext) {
			glfwMakeContextCurrent(oldCurrentContext);
		}
	}

	Renderer::~Renderer()
	{
		glDeleteVertexArrays(1, &m_pImpl->vaoID);

		delete m_pImpl;
	}

	auto Renderer::SetClearColor(F32 r, F32 g, F32 b, F32 a) -> void
	{
		glClearColor(r, g, b, a);
	}

	auto Renderer::Clear(Buffer buffer /*= Buffer(kColorBuffer | kDepthBuffer)*/) -> void
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

	auto Renderer::Flush() -> void
	{
		glBindVertexArray(m_pImpl->vaoID);

		m_pImpl->program.Bind();

		const auto view = m_pImpl->camera->ComputeViewMatrix();
		const auto vp = m_pImpl->projection * view;

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

			const auto ambient = glm::vec3(.1F, .1F, .1F) * glm::vec3(sect.material.diffuse * .25F);
			const auto light = Light {
				.position = { 1.0F, 1.0F, -1.0F },
				.diffuse = { .5F, .5F, .5F },
				.specular = { 1.0F, 1.0F, 1.0F },
				.attenuation = .2F
			};

			m_pImpl->program.UploadUniformMatrix4FV("u_vp", &(vp[0][0]));
			m_pImpl->program.UploadUniformMatrix4FV("u_model", &(sect.transform[0][0]));
			m_pImpl->program.UploadUniform3FV("u_ViewPos", &(m_pImpl->camera->Position()[0]));
			m_pImpl->program.UploadUniform3FV("u_Light.position", &(light.position[0]));
			m_pImpl->program.UploadUniform3FV("u_Ambient", &(ambient[0]));
			m_pImpl->program.UploadUniform3FV("u_Light.diffuse",  &(light.diffuse[0]));
			m_pImpl->program.UploadUniform3FV("u_Light.specular", &(light.specular[0]));
			m_pImpl->program.UploadUniform1F("u_Light.attenuation", .2F);
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

	auto Renderer::Render() -> void
	{
		Flush();
		glfwSwapBuffers(static_cast<GLFWwindow*>(Window().Handle()));

		m_pImpl->stats = m_pImpl->statsCurrent;
		m_pImpl->statsCurrent = RenderStats();
	}

	auto Renderer::MakeContextCurrent() -> void
	{
		glfwMakeContextCurrent(static_cast<GLFWwindow*>(Window().Handle()));
	}

	auto Renderer::Stats() -> RenderStats
	{
		return m_pImpl->stats;
	}

	auto Renderer::SetViewport(I32 x, I32 y, I32 width, I32 height) -> void
	{
		glViewport(x, y, width, height);
	}

	auto Renderer::SetProjection(glm::mat4 projection) -> void
	{
		m_pImpl->projection = std::move(projection);
	}

	auto Renderer::SetCamera(Mem::Shared<Camera> camera) -> void
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

	auto Renderer::DrawPoint(Vec3 p) -> void
	{
		DrawMesh({ {{ p }}, { 0 } }, PrimitiveMode::Points);
	}

	auto Renderer::DrawLine(Vec3 start, Vec3 end) -> void
	{
		DrawMesh({ {{ start }, { end }}, { 0, 1 } }, PrimitiveMode::Lines);
	}

	auto Renderer::DrawTri(const std::array<Vec3, 3>& ps) -> void
	{
		DrawMesh({ { { ps[0] }, { ps[1] }, { ps[2] } }, { 0, 1, 2 } }, PrimitiveMode::LineLoop);
	}

	auto Renderer::FillTri(const std::array<Vec3, 3>& ps) -> void
	{
		DrawMesh({ { { ps[0] }, { ps[1] }, { ps[2] } }, { 0, 1, 2 } }, PrimitiveMode::Triangles);
	}
}

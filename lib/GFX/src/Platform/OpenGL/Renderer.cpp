#include "GFX/Platform/OpenGL/Renderer.hpp"

#include "GFX/Platform/OpenGL/Objects/IndexBuffer.hpp"
#include "GFX/Platform/OpenGL/Objects/Shader.hpp"
#include "GFX/Platform/OpenGL/Objects/VertexBuffer.hpp"

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
	};

	struct Renderer::Impl
	{
		GLID vaoID;
		Objects::ShaderProgram program;
		Objects::VertexBuffer vertexBuf;
		Objects::IndexBuffer indexBuf;
		std::vector<BufferSection> vertexBufSects;
		std::vector<BufferSection> indexBufSects;
	};

	static constexpr auto kStaticBufferSize = 8 * 1024 * 1024; // 8 MiB

	Renderer::Renderer(Mem::Shared<WM::Window> window)
		: GFX::Renderer(std::move(window))
		, m_pImpl(nullptr)
	{
		const auto currentContextExists = glfwGetCurrentContext() != nullptr;
		if (!currentContextExists) {
			glfwMakeContextCurrent(static_cast<GLFWwindow*>(Window().Handle()));
		}

		gladLoadGL(static_cast<GLADloadfunc>(glfwGetProcAddress));

#ifndef NDEBUG
		EnableDebugOutput();
#endif

		const auto* vertexSource = R"(
			#version 330 core
			layout (location = 0) in vec3 aPos;

			void main()
			{
				gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
			}
		)";
		const auto* fragmentSource = R"(
			#version 330 core
			out vec4 FragColor;

			void main()
			{
				FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
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
			Objects::VertexBuffer(nullptr, kStaticBufferSize),
			Objects::IndexBuffer(nullptr, kStaticBufferSize),
			{},
			{}
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));
		glEnableVertexAttribArray(0);
		glBindVertexArray(GLID(oldVAO));

		if (!currentContextExists) {
			glfwMakeContextCurrent(nullptr);
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

	auto Renderer::Clear(Buffer buffer /*= kColorBuffer*/) -> void
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
		auto vaoBinding = 0;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vaoBinding);
		auto oldVAOID = GLID(vaoBinding);

		if (oldVAOID != m_pImpl->vaoID) {
			glBindVertexArray(m_pImpl->vaoID);
		}

		m_pImpl->program.Bind();

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

			glDrawElementsBaseVertex(
				drawMode,
				sect.size / Mesh::kIndexSize,
				GL_UNSIGNED_INT,
				reinterpret_cast<void*>(sect.offset),
				m_pImpl->vertexBufSects[i].offset / Mesh::kVertexSize
			);
		}
		if (oldVAOID != m_pImpl->vaoID) {
			glBindVertexArray(oldVAOID);
		}

		m_pImpl->vertexBufSects.clear();
		m_pImpl->indexBufSects.clear();
	}

	auto Renderer::Render() -> void
	{
		Flush();
		glfwSwapBuffers(static_cast<GLFWwindow*>(Window().Handle()));
	}

	auto Renderer::MakeContextCurrent() -> void
	{
		glfwMakeContextCurrent(static_cast<GLFWwindow*>(Window().Handle()));
	}

	auto Renderer::SetViewport(I32 x, I32 y, I32 width, I32 height) -> void
	{
		glViewport(x, y, width, height);
	}

	auto Renderer::SetProjection(glm::mat4 projection) -> void
	{

	}

	auto Renderer::SetCamera(Mem::Shared<Camera> camera) -> void
	{

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
					mode
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
					mode
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

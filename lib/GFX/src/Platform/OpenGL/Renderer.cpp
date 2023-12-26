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

	struct Renderer::Impl
	{
		GLID vaoID;
	};
	Renderer::Renderer(Mem::Shared<WM::Window> window)
		: GFX::Renderer(std::move(window))
		, m_pImpl(new Impl())
	{
		const auto currentContextExists = glfwGetCurrentContext() != nullptr;
		if (!currentContextExists) {
			glfwMakeContextCurrent(static_cast<GLFWwindow*>(Window().Handle()));
		}

		gladLoadGL(static_cast<GLADloadfunc>(glfwGetProcAddress));

#ifndef NDEBUG
		EnableDebugOutput();
#endif
		glEnable(GL_PROGRAM_POINT_SIZE);

		glGenVertexArrays(1, &m_pImpl->vaoID);

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

	auto Renderer::Render() -> void
	{
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
		glBindVertexArray(m_pImpl->vaoID);

		auto vbo = Objects::VertexBuffer(
			mesh.Vertices().data(),
			I64(mesh.Vertices().size() * sizeof(mesh.Vertices()[0]))
		);
		vbo.Bind();

		auto ebo = Objects::IndexBuffer(
			mesh.Indices().data(),
			I64(mesh.Indices().size() * sizeof(mesh.Indices()[0]))
		);
		ebo.Bind();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		const auto* vertexSource = R"(
			#version 330 core
			layout (location = 0) in vec3 aPos;

			void main()
			{
				gl_PointSize = 5;
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

		auto prog = Objects::ShaderProgram({ &vShader, &fShader });
		if (!prog.Link()) {
			const auto log = prog.RetrieveErrorLog(512);
			fprintf(stderr, "Error linking shader program: %s\n", log.c_str());
			return;
		}
		prog.Bind();

		switch (mode) {
		case PrimitiveMode::Points:
			glDrawElements(GL_POINTS         , I32(mesh.Indices().size()), GL_UNSIGNED_INT, 0);
			break;
		case PrimitiveMode::Lines:
			glDrawElements(GL_LINES          , I32(mesh.Indices().size()), GL_UNSIGNED_INT, 0);
			break;
		case PrimitiveMode::LineStrip:
			glDrawElements(GL_LINE_STRIP     , I32(mesh.Indices().size()), GL_UNSIGNED_INT, 0);
			break;
		case PrimitiveMode::LineLoop:
			glDrawElements(GL_LINE_LOOP      , I32(mesh.Indices().size()), GL_UNSIGNED_INT, 0);
			break;
		case PrimitiveMode::Triangles:
			glDrawElements(GL_TRIANGLES      , I32(mesh.Indices().size()), GL_UNSIGNED_INT, 0);
			break;
		case PrimitiveMode::TriangleStrip:
			glDrawElements(GL_TRIANGLE_STRIP , I32(mesh.Indices().size()), GL_UNSIGNED_INT, 0);
			break;
		case PrimitiveMode::TriangleFan:
			glDrawElements(GL_TRIANGLE_FAN   , I32(mesh.Indices().size()), GL_UNSIGNED_INT, 0);
			break;
		}
	}

	auto Renderer::DrawPoint(Vec3 p) -> void
	{

	}

	auto Renderer::DrawLine(Vec3 start, Vec3 end) -> void
	{

	}

	auto Renderer::DrawTri(const std::array<Vec3, 3>& ps) -> void
	{

	}

	auto Renderer::FillTri(const std::array<Vec3, 3>& ps) -> void
	{

	}
}

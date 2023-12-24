#include "GFX/Platform/OpenGL/Renderer.hpp"

#include "glad/gl.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>

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

	Renderer::Renderer(Mem::Shared<WM::Window> window)
		: GFX::Renderer(std::move(window))
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

		if (!currentContextExists) {
			glfwMakeContextCurrent(nullptr);
		}
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
		auto vao = GLID(0);
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		auto vbo = GLID(0);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(
			GL_ARRAY_BUFFER,
			I64(mesh.Vertices().size() * sizeof(mesh.Vertices()[0])),
			mesh.Vertices().data(),
			GL_STATIC_DRAW
		);

		auto ebo = GLID(0);
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			I64(mesh.Indices().size() * sizeof(mesh.Indices()[0])),
			mesh.Indices().data(),
			GL_STATIC_DRAW
		);

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

		auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, nullptr);
		glCompileShader(vertexShader);
		auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
		glCompileShader(fragmentShader);
		auto program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		glUseProgram(program);

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

		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
		glDeleteProgram(program);
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

#include "GFX/Platform/OpenGL/Renderer.hpp"

#include "glad/gl.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>

namespace Gaze::GFX::Platform::OpenGL {
	Renderer::Renderer(Mem::Shared<WM::Window> window)
		: GFX::Renderer(std::move(window))
	{
		const auto currentContextExists = glfwGetCurrentContext() != nullptr;
		if (!currentContextExists) {
			glfwMakeContextCurrent(static_cast<GLFWwindow*>(Window().Handle()));
		}

		gladLoadGL(static_cast<GLADloadfunc>(glfwGetProcAddress));

		if (!currentContextExists) {
			glfwMakeContextCurrent(nullptr);
		}
	}

	auto Renderer::SetColor(F32 r, F32 g, F32 b, F32 a) -> void
	{

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

	auto Renderer::SetViewport(I32 x, I32 y, I32 width, I32 height) -> void
	{

	}

	auto Renderer::SetProjection(glm::mat4 projection) -> void
	{

	}

	auto Renderer::SetCamera(Mem::Shared<Camera> camera) -> void
	{

	}

	auto Renderer::DrawMesh(const Mesh& mesh, PrimitiveMode mode) -> void
	{

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

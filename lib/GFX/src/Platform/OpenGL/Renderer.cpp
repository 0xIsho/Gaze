#include "GFX/Platform/OpenGL/Renderer.hpp"

#include "glad/gl.h"
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

	}

	auto Renderer::Clear() -> void
	{

	}

	auto Renderer::Render() -> void
	{

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

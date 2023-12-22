#pragma once

#include "GFX/Renderer.hpp"

namespace Gaze::GFX::Platform::OpenGL {
	class Renderer : public GFX::Renderer
	{
	public:
		Renderer(Mem::Shared<WM::Window> window);
		virtual ~Renderer() = default;

		auto SetClearColor(F32 r, F32 g, F32 b, F32 a) -> void override;
		auto Clear(Buffer buffer = kColorBuffer) -> void override;
		auto Render() -> void override;
		auto SetViewport(I32 x, I32 y, I32 width, I32 height) -> void override;
		auto SetProjection(glm::mat4 projection) -> void override;
		auto SetCamera(Mem::Shared<Camera> camera) -> void override;

		auto DrawMesh(const Mesh& mesh, PrimitiveMode mode) -> void override;
		auto DrawPoint(Vec3 p) -> void override;
		auto DrawLine(Vec3 start, Vec3 end) -> void override;
		auto DrawTri(const std::array<Vec3, 3>& ps) -> void override;
		auto FillTri(const std::array<Vec3, 3>& ps) -> void override;
	};
}

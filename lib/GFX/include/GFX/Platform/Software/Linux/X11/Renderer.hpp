#pragma once

#include "GFX/Renderer.hpp"

namespace Gaze::GFX::Platform::Software::Linux::X11 {
	class Renderer : public GFX::Renderer
	{
		struct Impl;

	public:
		Renderer(Mem::Shared<WM::Window> window);
		~Renderer();

		auto SetColor(F32 r, F32 g, F32 b, F32 a) -> void override;
		auto SetClearColor(F32 r, F32 g, F32 b, F32 a) -> void override;
		auto Clear() -> void override;
		auto Render() -> void override;
		auto SetViewport(I32 x, I32 y, I32 width, I32 height) -> void override;

		auto DrawMesh(const Mesh& mesh, PrimitiveMode mode) -> void override;

		auto DrawPoint(Vec3 p) -> void override;
		auto DrawLine(Vec3 start, Vec3 end) -> void override;
		auto DrawTri(const std::array<Vec3, 3>& ps) -> void override;
		auto FillTri(const std::array<Vec3, 3>& ps) -> void override;

	private:
		auto NDCtoScreen(glm::vec3& vec) -> void;

	private:
		Impl* m_pImpl{ nullptr };
	};
}

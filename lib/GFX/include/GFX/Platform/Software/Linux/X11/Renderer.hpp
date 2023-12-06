#pragma once

#include "GFX/Renderer.hpp"

namespace Gaze::GFX::Platform::Software::Linux::X11 {
	class Renderer : public GFX::Renderer
	{
		struct Impl;

	public:
		Renderer(Mem::Shared<WM::Window> window);
		~Renderer();

		auto SetClearColor(F32 r, F32 g, F32 b, F32 a) -> void override;
		auto Clear() -> void override;
		auto Render() -> void override;

		auto DrawPoint(Vec4 p) -> void override;
		auto DrawLine(Vec4 start, Vec4 end) -> void override;
		auto DrawTri(const std::array<Vec4, 3>& ps) -> void override;
		auto FillTri(const std::array<Vec4, 3>& ps) -> void override;

	private:
		auto NDCtoScreen(glm::vec4& vec) -> void;

	private:
		Impl* m_pImpl{ nullptr };
	};
}

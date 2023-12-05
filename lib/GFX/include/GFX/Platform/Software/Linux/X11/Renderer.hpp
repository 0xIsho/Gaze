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

		auto DrawPoint(Point2D p) -> void override;
		auto DrawLine(Point2D start, Point2D end) -> void override;
		auto DrawTri(const std::array<Point2D, 3>& ps) -> void override;
		auto FillTri(const std::array<Point2D, 3>& ps) -> void override;

	private:
		Impl* m_pImpl{ nullptr };
	};
}

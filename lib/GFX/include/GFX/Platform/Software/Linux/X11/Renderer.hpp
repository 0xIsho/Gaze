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

		auto DrawPoint(F32 x, F32 y) -> void override;
		auto DrawLine(F32 x1, F32 y1, F32 x2, F32 y2) -> void override;

	private:
		Impl* m_pImpl{ nullptr };
	};
}

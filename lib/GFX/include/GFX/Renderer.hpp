#pragma once

#include "Core/Type.hpp"

#include "GFX/API.hpp"

#include "WM/Window.hpp"

#include <array>

namespace Gaze::GFX {
	struct Point2D
	{
		F32 x;
		F32 y;
	};

	class Renderer
	{
	public:
		Renderer(Mem::Shared<WM::Window> window);
		virtual ~Renderer() = default;

		virtual auto SetClearColor(F32 r, F32 g, F32 b, F32 a) -> void = 0;
		virtual auto Clear() -> void = 0;
		virtual auto Render() -> void = 0;

		/* ********** 2D Primitive Rendering Functions ********** */
		// Coordinates are in the range [0.0F-1.0F]!

		virtual auto DrawPoint(Point2D p) -> void = 0;
		virtual auto DrawLine(Point2D start, Point2D end) -> void = 0;
		virtual auto DrawTri(const std::array<Point2D, 3>& ps) -> void = 0;
		/**********************************************************/

	protected:
		[[nodiscard]]
		auto Window() const -> const WM::Window& { return *m_Window.get(); }
		[[nodiscard]]
		auto Window() -> WM::Window& { return *m_Window.get(); }

	private:
		Mem::Shared<WM::Window> m_Window;
	};

	auto CreateRenderer(Mem::Shared<WM::Window> window) -> Mem::Unique<Renderer>;
}

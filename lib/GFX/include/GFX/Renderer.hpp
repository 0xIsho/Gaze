#pragma once

#include "Core/Type.hpp"

#include "GFX/API.hpp"

#include "WM/Window.hpp"

#include <glm/vec4.hpp>

#include <array>

namespace Gaze::GFX {
	using Vec4 = glm::vec4;

	class Renderer
	{
	public:
		Renderer(Mem::Shared<WM::Window> window);
		virtual ~Renderer() = default;

		virtual auto SetClearColor(F32 r, F32 g, F32 b, F32 a) -> void = 0;
		virtual auto Clear() -> void = 0;
		virtual auto Render() -> void = 0;

		/* ********** 2D Primitive Rendering Functions ********** */
		// Coordinates are NDC [-1.0F,1.0F]!

		virtual auto DrawPoint(Vec4 p) -> void = 0;
		virtual auto DrawLine(Vec4 start, Vec4 end) -> void = 0;
		virtual auto DrawTri(const std::array<Vec4, 3>& ps) -> void = 0;
		virtual auto FillTri(const std::array<Vec4, 3>& ps) -> void = 0;
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

#pragma once

#include "Core/Type.hpp"

#include "GFX/API.hpp"

#include "WM/Window.hpp"

namespace Gaze::GFX {
	class Renderer
	{
	public:
		Renderer(Mem::Shared<WM::Window> window);
		virtual ~Renderer() = default;

		virtual auto SetClearColor(F32 r, F32 g, F32 b, F32 a) -> void = 0;
		virtual auto Clear() -> void = 0;
		virtual auto Render() -> void = 0;

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

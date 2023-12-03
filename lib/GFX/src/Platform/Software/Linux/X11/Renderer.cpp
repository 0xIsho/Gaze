#include "GFX/Platform/Software/Linux/X11/Renderer.hpp"

namespace Gaze::GFX::Platform::Software::Linux::X11 {
	Renderer::Renderer(Mem::Shared<WM::Window> window)
		: GFX::Renderer(std::move(window))
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
}

#include "GFX/Renderer.hpp"

#include "Core/Type.hpp"
#include "Core/PlatformUtils.hpp"

#include "GFX/API.hpp"
#include "GFX/Platform/OpenGL/Renderer.hpp"

namespace Gaze::GFX {
	Renderer::Renderer(Shared<WM::Window> window) noexcept
		: m_Window(std::move(window))
	{
	}

	auto CreateRenderer(Shared<WM::Window> window) -> Unique<Renderer>
	{
		switch (GetAPI()) {
		case API::kOpenGL: return MakeUnique<Platform::OpenGL::Renderer>(std::move(window));
		}

		GAZE_UNREACHABLE();
	}
}

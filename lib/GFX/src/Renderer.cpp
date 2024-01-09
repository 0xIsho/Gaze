#include "GFX/Renderer.hpp"

#include "Core/Type.hpp"
#include "Core/PlatformUtils.hpp"

#include "GFX/API.hpp"
#include "GFX/Platform/OpenGL/Renderer.hpp"

namespace Gaze::GFX {
	Renderer::Renderer(Mem::Shared<WM::Window> window) noexcept
		: m_Window(std::move(window))
	{
	}

	auto CreateRenderer(Mem::Shared<WM::Window> window) -> Mem::Unique<Renderer>
	{
		switch (GetAPI()) {
		case API::kOpenGL: return Mem::MakeUnique<Platform::OpenGL::Renderer>(std::move(window));
		}

		GAZE_UNREACHABLE();
	}
}

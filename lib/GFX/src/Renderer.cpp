#include "GFX/Renderer.hpp"

#include "Core/Type.hpp"
#include "Core/Platform.hpp"

#include "GFX/API.hpp"
#include "GFX/Platform/Software/Renderer.hpp"

namespace Gaze::GFX {
	Renderer::Renderer(Mem::Shared<WM::Window> window)
		: m_Window(std::move(window))
	{
	}

	auto CreateRenderer(Mem::Shared<WM::Window> window) -> Mem::Unique<Renderer>
	{
		switch (GetAPI()) {
		case API::kSoftware: return Mem::MakeUnique<Platform::Software::Renderer>(std::move(window)); break;
		}

#if GAZE_COMPILER_CLANG == 1 || GAZE_COMPILER_GNU == 1
		__builtin_unreachable();
#else
#	error "__builtin_unreachable() equivalent missing"
#endif

		return nullptr;
	}
}

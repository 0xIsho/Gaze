#include "Core/Platform.hpp"

#if defined(GAZE_PLATFORM_LINUX)
#	include "Linux/Renderer.hpp"
#endif

namespace Gaze::GFX::Platform::Software {
#if defined(GAZE_PLATFORM_LINUX)
	using Renderer = Linux::Renderer;
#else
#	error "Unsupported platform!"
#endif
}

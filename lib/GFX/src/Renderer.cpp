#include "GFX/Renderer.hpp"

#include "Core/Type.hpp"
#include "Core/Platform.hpp"

#include "GFX/API.hpp"
#include "GFX/Platform/Software/Renderer.hpp"

namespace Gaze::GFX {
	auto CreateRenderer() -> Mem::Unique<Renderer>
	{
		switch (GetAPI()) {
		case API::kSoftware: return Mem::MakeUnique<Platform::Software::Renderer>(); break;
		}

#if GAZE_COMPILER_CLANG == 1 || GAZE_COMPILER_GNU == 1
		__builtin_unreachable();
#else
#	error "__builtin_unreachable() equivalent missing"
#endif

		return nullptr;
	}
}

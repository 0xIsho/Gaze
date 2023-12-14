#include "Net/Core.hpp"

#include "enet/enet.h"

namespace Gaze::Net {
	static auto s_IsInitialized = false;

	auto Init() -> bool
	{
		return s_IsInitialized = (enet_initialize() == 0);
	}

	auto IsInitialized() -> bool
	{
		return s_IsInitialized;
	}

	auto Terminate() -> void
	{
		enet_deinitialize();
		s_IsInitialized = false;
	}
}

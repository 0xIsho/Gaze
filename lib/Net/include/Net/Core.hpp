#pragma once

namespace Gaze::Net {
	[[nodiscard]]
	auto Init() -> bool;
	auto IsInitialized() -> bool;
	auto Terminate() -> void;
}

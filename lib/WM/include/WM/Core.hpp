#pragma once

namespace Gaze::WM {
	[[nodiscard]] auto Init() -> bool;
	auto Terminate() -> void;
	[[nodiscard]] auto IsInitialized() -> bool;
}

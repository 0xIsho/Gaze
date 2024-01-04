#pragma once

namespace Gaze::WM {
	[[nodiscard]] auto Init()          noexcept -> bool;
	              auto Terminate()     noexcept -> void;
	[[nodiscard]] auto IsInitialized() noexcept -> bool;
	              auto PollEvents()    noexcept -> void;
}

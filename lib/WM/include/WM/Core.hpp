#pragma once

namespace Gaze::WM {
	/**
	 * @brief Initialise the Window Manager
	 *
	 * This function MUST be called first before using any library functionality
	 *
	 * @return true if the library was successfully initialised, false otherwise
	 */
	[[nodiscard]] auto Init()          noexcept -> bool;

	/**
	 * @brief Terminates the Window Manager and releases any associated resources
	 */
	              auto Terminate()     noexcept -> void;

	/**
	 * @brief Check whether the Window Manager is initialised or not
	 *
	 * @return true if the WM is initialised, false otherwise
	 */
	[[nodiscard]] auto IsInitialized() noexcept -> bool;

	/**
	 * @brief Dispatch new events received from the underlying windowing system
	 *
	 * This function should be called regularly to make sure that windows remain
	 * responsive.
	 */
	              auto PollEvents()    noexcept -> void;
}

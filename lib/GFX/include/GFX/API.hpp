#pragma once

namespace Gaze::GFX {
	/**
	 * @brief The API to use for rendering.
	 */
	enum class API {
		kOpenGL
	};

	/**
	 * @brief Set the graphics API.
	 */
	auto SetAPI(API api) noexcept -> void;
	/**
	 * @brief Get the graphics API.
	 */
	auto GetAPI()        noexcept -> API;
}

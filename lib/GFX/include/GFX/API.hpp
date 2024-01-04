#pragma once

namespace Gaze::GFX {
	enum class API {
		kOpenGL
	};

	auto SetAPI(API api) noexcept -> void;
	auto GetAPI()        noexcept -> API;
}

#pragma once

namespace Gaze::GFX {
	enum class API {
		kSoftware,
		kOpenGL
	};

	auto SetAPI(API api) -> void;
	auto GetAPI() -> API;
}

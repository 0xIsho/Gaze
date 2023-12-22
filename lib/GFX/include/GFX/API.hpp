#pragma once

namespace Gaze::GFX {
	enum class API {
		kOpenGL
	};

	auto SetAPI(API api) -> void;
	auto GetAPI() -> API;
}

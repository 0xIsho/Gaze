#pragma once

namespace Gaze::GFX {
	enum class API {
		kSoftware,
	};

	auto SetAPI(API api) -> void;
	auto GetAPI() -> API;
}

#include "GFX/API.hpp"

namespace Gaze::GFX {
	static auto s_API = API::kSoftware;

	auto SetAPI(API api) -> void {
		s_API = api;
	}

	auto GetAPI() -> API
	{
		return s_API;
	}
}

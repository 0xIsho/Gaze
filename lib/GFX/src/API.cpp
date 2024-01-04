#include "GFX/API.hpp"

namespace Gaze::GFX {
	static auto s_API = API::kOpenGL;

	auto SetAPI(API api) noexcept -> void {
		s_API = api;
	}

	auto GetAPI() noexcept -> API
	{
		return s_API;
	}
}

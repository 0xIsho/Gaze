#pragma once

#include "Event.hpp"

namespace Gaze::Events {
	class WindowClose : public Event
	{
	public:
		GAZE_REGISTER_EVENT(WindowClose);
	};
}

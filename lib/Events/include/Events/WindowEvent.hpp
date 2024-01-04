#pragma once

#include "Event.hpp"

namespace Gaze::Events {
	class WindowClose : public Event
	{
	public:
		GAZE_REGISTER_EVENT(WindowClose);
	};

	class WindowFocus : public Event
	{
	public:
		GAZE_REGISTER_EVENT(WindowFocus);

		WindowFocus(bool focused) noexcept
			: m_Focused(focused)
		{}

		[[nodiscard]]
		auto IsFocused() const noexcept -> bool { return m_Focused; }

	private:
		bool m_Focused;
	};
}

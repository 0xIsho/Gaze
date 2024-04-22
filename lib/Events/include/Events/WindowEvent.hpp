#pragma once

#include "Event.hpp"

namespace Gaze::Events {
	/**
	 * @brief Represents a window close event
	 *
	 * This event is triggered when the window is *requested* to close,
	 * but not necessarily when it is actually closed.
	 */
	class WindowClose : public Event
	{
	public:
		GAZE_REGISTER_EVENT(WindowClose);
	};

	/**
	 * @brief Represents a change of focus event
	 */
	class WindowFocus : public Event
	{
	public:
		GAZE_REGISTER_EVENT(WindowFocus);

		/**
		 * @brief Construct a new Window Focus object
		 *
		 * @param focused Whether the window is focused or not
		 */
		WindowFocus(bool focused) noexcept
			: m_Focused(focused)
		{}

		/**
		 * @brief Whether the window is focused or not
		 *
		 * @return true If it is focused, false otherwise
		 */
		[[nodiscard]] auto IsFocused() const noexcept -> bool { return m_Focused; }

	private:
		bool m_Focused; /**< Whether the window is focused or not */
	};
}

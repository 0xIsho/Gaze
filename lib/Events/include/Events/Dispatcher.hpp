#pragma once

#include "Event.hpp"

#include <utility>

namespace Gaze::Events {
	/**
	 * @brief Dispatches an event to a callback function.
	 */
	class Dispatcher
	{
	public:
		/**
		 * @brief Constructs a dispatcher for the given event.
		 *
		 * @param event The event to dispatch.
		 */
		Dispatcher(Event& event) noexcept
			: m_Event(event)
		{}

		/**
		 * @brief Dispatches the event to the given callback function.
		 *
		 * @tparam TEvent The type of the triggering event.
		 * @tparam TFunc The type of the callback function.
		 * @tparam TArgs The types of the arguments to the callback function.
		 *
		 * @param func The callback function.
		 * @param args The arguments to the callback function.
		 *
		 * @return true if the event was handled by the callback function.
		 *
		 * @note The event is set to handled when it is dispatched to the
		 *   callback function. If that is not desired, call `SetHandled(false)`
		 *   on the event object before returning from the callback.
		 */
		template<typename TEvent, typename TFunc, typename... TArgs>
		auto Dispatch(TFunc&& func, TArgs&&... args) -> bool
		{
			if (!m_Event.Handled() && TEvent::kEventType == m_Event.GetType()) {
				// Set this before calling the callback to prevent this call
				// from overriding "Handled()" calls made in the callback.
				m_Event.SetHandled(true);

				func(static_cast<TEvent&>(m_Event), std::forward<TArgs>(args)...);
			}

			return m_Event.Handled();
		}

	private:
		Event& m_Event; /**< The event to dispatch. */
	};
}

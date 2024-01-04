#pragma once

#include "Event.hpp"

#include <utility>

namespace Gaze::Events {
	class Dispatcher
	{
	public:
		Dispatcher(Event& event) noexcept
			: m_Event(event)
		{}

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
		Event& m_Event;
	};
}

#pragma once

#include "Event.hpp"

namespace Gaze::Events {
	class MouseMove : public Event
	{
	public:
		GAZE_REGISTER_EVENT(MouseMove)

	public:
		MouseMove(double x, double y)
			: m_X(x)
			, m_Y(y)
		{}

		[[nodiscard]]
		auto X() const { return m_X; }
		[[nodiscard]]
		auto Y() const { return m_Y; }

	private:
		double m_X;
		double m_Y;
	};
}

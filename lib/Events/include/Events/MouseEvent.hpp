#pragma once

#include "Event.hpp"

#include "Input/KeyCode.hpp"

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

	class MouseButton : public Event
	{
	public:
		MouseButton(Input::MouseButton button, Input::Mod mods)
			: m_Button(button)
			, m_Mods(mods)
		{}

		[[nodiscard]]
		auto Button() const { return m_Button; }
		[[nodiscard]]
		auto Mods() const { return m_Mods; }

	private:
		Input::MouseButton m_Button;
		Input::Mod m_Mods;
	};

	class MouseButtonPressed : public MouseButton
	{
	public:
		GAZE_REGISTER_EVENT(MouseButtonPressed);

	public:
		MouseButtonPressed(Input::MouseButton button, Input::Mod mods)
			: MouseButton(button, mods)
		{}
	};

	class MouseButtonReleased : public MouseButton
	{
	public:
		GAZE_REGISTER_EVENT(MouseButtonReleased);

	public:
		MouseButtonReleased(Input::MouseButton button, Input::Mod mods)
			: MouseButton(button, mods)
		{}
	};
}

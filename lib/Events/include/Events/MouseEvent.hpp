#pragma once

#include "Event.hpp"

#include "Input/KeyCode.hpp"

namespace Gaze::Events {
	class MouseMove : public Event
	{
	public:
		GAZE_REGISTER_EVENT(MouseMove)

	public:
		MouseMove(double x, double y) noexcept
			: m_X(x)
			, m_Y(y)
		{}

		[[nodiscard]] auto X() const noexcept { return m_X; }
		[[nodiscard]] auto Y() const noexcept { return m_Y; }

	private:
		double m_X;
		double m_Y;
	};

	class MouseButton : public Event
	{
	public:
		MouseButton(Input::MouseButton button, Input::Mod mods) noexcept
			: m_Button(button)
			, m_Mods(mods)
		{}

		[[nodiscard]] auto Button() const noexcept { return m_Button; }
		[[nodiscard]] auto Mods()   const noexcept { return m_Mods; }

	private:
		Input::MouseButton m_Button;
		Input::Mod m_Mods;
	};

	class MouseButtonPressed : public MouseButton
	{
	public:
		GAZE_REGISTER_EVENT(MouseButtonPressed);

	public:
		MouseButtonPressed(Input::MouseButton button, Input::Mod mods) noexcept
			: MouseButton(button, mods)
		{}
	};

	class MouseButtonReleased : public MouseButton
	{
	public:
		GAZE_REGISTER_EVENT(MouseButtonReleased);

	public:
		MouseButtonReleased(Input::MouseButton button, Input::Mod mods) noexcept
			: MouseButton(button, mods)
		{}
	};
}

namespace Gaze {
	[[nodiscard]]
	inline auto ToString(const Events::MouseMove& event) -> std::string
	{
		return std::format(
			"{}\n"
			"  - Type: MouseMove\n"
			"  - x: {}\n"
			"  - y: {}",
			ToString(static_cast<Events::Event>(event)),
			event.X(),
			event.Y()
		);
	}

	[[nodiscard]]
	inline auto ToString(const Events::MouseButton& event) -> std::string
	{
		return std::format(
			"{}\n"
			"  - Type: MouseButton\n"
			"  - Button: {}\n"
			"  - Mods  :\n"
			"    - kModShift    : {}\n"
			"    - kModControl  : {}\n"
			"    - kModAlt      : {}\n"
			"    - kModSuper    : {}\n"
			"    - kModCapsLock : {}\n"
			"    - kModNumLock  : {}",
			ToString(static_cast<Events::Event>(event)),
			[&event] {
				switch (event.Button()) {
				case Input::MouseButton::k1: return "0 (Left)";
				case Input::MouseButton::k2: return "1 (Right)";
				case Input::MouseButton::k3: return "2 (Middle)";
				case Input::MouseButton::k4: return "3";
				case Input::MouseButton::k5: return "4";
				case Input::MouseButton::k6: return "5";
				case Input::MouseButton::k7: return "6";
				case Input::MouseButton::k8: return "7";
				}
				return "Unknown";
			}(),
			event.Mods() & Input::Mod::kModShift    ? "On" : "Off",
			event.Mods() & Input::Mod::kModControl  ? "On" : "Off",
			event.Mods() & Input::Mod::kModAlt      ? "On" : "Off",
			event.Mods() & Input::Mod::kModSuper    ? "On" : "Off",
			event.Mods() & Input::Mod::kModCapsLock ? "On" : "Off",
			event.Mods() & Input::Mod::kModNumLock  ? "On" : "Off"
		);
	}

	[[nodiscard]]
	inline auto ToString(const Events::MouseButtonPressed& event) -> std::string
	{
		return std::format("{}\n  - State: Pressed", ToString(static_cast<Events::MouseButton>(event)));
	}

	[[nodiscard]]
	inline auto ToString(const Events::MouseButtonReleased& event) -> std::string
	{
		return std::format("{}\n  - State: Released", ToString(static_cast<Events::MouseButton>(event)));
	}
}

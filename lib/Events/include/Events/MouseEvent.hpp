#pragma once

#include "Event.hpp"

#include "Input/KeyCode.hpp"

namespace Gaze::Events {
	/**
	 * @brief Represents a mouse move event
	 *
	 * This event is triggered when the cursor is moved around in the window.
	 */
	class MouseMove : public Event
	{
	public:
		GAZE_REGISTER_EVENT(MouseMove)

	public:
		/**
		 * @brief Constructs a new MouseMove event.
		 *
		 * @param x The x coordinate of the cursor.
		 * @param y The y coordinate of the cursor.
		 */
		MouseMove(double x, double y) noexcept
			: m_X(x)
			, m_Y(y)
		{}

		/**
		 * @brief Returns the x coordinate of the cursor.
		 *
		 * @return The x coordinate of the cursor.
		 */
		[[nodiscard]] auto X() const noexcept { return m_X; }
		/**
		 * @brief Returns the y coordinate of the cursor.
		 *
		 * @return The y coordinate of the cursor.
		 */
		[[nodiscard]] auto Y() const noexcept { return m_Y; }

	private:
		double m_X; /**< The x coordinate of the cursor.  */
		double m_Y; /**< The y coordinate of the cursor.  */
	};

	/**
	 * @brief Represents a mouse button state change event
	 */
	class MouseButton : public Event
	{
	public:
		/**
		 * @brief Constructs a new MouseButton event.
		 *
		 * @param button The button that was clicked.
		 * @param mods The modifiers that were pressed when the button was clicked. 
		 */
		MouseButton(Input::MouseButton button, Input::Mod mods) noexcept
			: m_Button(button)
			, m_Mods(mods)
		{}

		/**
		 * @brief Returns the button that was clicked.
		 *
		 * @return The button that was clicked.
		 */
		[[nodiscard]] auto Button() const noexcept { return m_Button; }
		/**
		 * @brief Returns the modifiers that were pressed when the button was clicked.
		 *
		 * @return The modifiers that were pressed when the button was clicked.
		 */
		[[nodiscard]] auto Mods() const noexcept { return m_Mods; }

	private:
		Input::MouseButton m_Button; /**< The button that was clicked. */
		Input::Mod         m_Mods;   /**< The modifiers that were pressed when the button was clicked. */
	};

	/**
	 * @brief Represents a mouse click event
	 */
	class MouseButtonPressed : public MouseButton
	{
	public:
		GAZE_REGISTER_EVENT(MouseButtonPressed);

	public:
		/**
		 * @brief Constructs a new MouseButtonPressed event.
		 *
		 * @param button The button that was clicked.
		 * @param mods The modifiers that were pressed when the button was clicked.
		 */
		MouseButtonPressed(Input::MouseButton button, Input::Mod mods) noexcept
			: MouseButton(button, mods)
		{}
	};

	/**
	 * @brief Represents a mouse release event
	 */
	class MouseButtonReleased : public MouseButton
	{
	public:
		GAZE_REGISTER_EVENT(MouseButtonReleased);

	public:
		/**
		 * @brief Constructs a new MouseButtonReleased event.
		 *
		 * @param button The button that was released.
		 * @param mods The modifiers that were pressed when the button was released.
		 */
		MouseButtonReleased(Input::MouseButton button, Input::Mod mods) noexcept
			: MouseButton(button, mods)
		{}
	};
}

namespace Gaze {
	/**
	 * @brief Returns the string representation of the event.
	 *
	 * @param event The event.
	 *
	 * @return The string representation of the event.
	 */
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

	/**
	 * @brief Returns the string representation of the event.
	 *
	 * @param event The event.
	 *
	 * @return The string representation of the event.
	 */
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

	/**
	 * @brief Returns the string representation of the event.
	 *
	 * @param event The event.
	 *
	 * @return The string representation of the event.
	 */
	[[nodiscard]]
	inline auto ToString(const Events::MouseButtonPressed& event) -> std::string
	{
		return std::format("{}\n  - State: Pressed", ToString(static_cast<Events::MouseButton>(event)));
	}

	/**
	 * @brief Returns the string representation of the event.
	 *
	 * @param event The event.
	 *
	 * @return The string representation of the event.
	 */
	[[nodiscard]]
	inline auto ToString(const Events::MouseButtonReleased& event) -> std::string
	{
		return std::format("{}\n  - State: Released", ToString(static_cast<Events::MouseButton>(event)));
	}
}

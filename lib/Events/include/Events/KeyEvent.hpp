#pragma once

#include "Event.hpp"

#include "Input/KeyCode.hpp"

namespace Gaze::Events {
	/**
	 * @brief The Key class represents a key event.
	 */
	class Key : public Event
	{
	public:
		/**
		 * @brief The Key constructor.
		 *
		 * @param key The keycode.
		 * @param scancode The scancode.
		 * @param mods The mods.
		 */
		Key(Input::Key key, int scancode, Input::Mod mods) noexcept
			: m_Key(key)
			, m_Scancode(scancode)
			, m_Mods(mods)
		{}

		/*
		 * @brief Returns the keycode.
		 *
		 * @return The keycode.
		 */
		[[nodiscard]] auto Keycode()  const noexcept { return m_Key; }
		/**
		 * @brief Returns the scancode.
		 *
		 * @return The scancode.
		 */
		[[nodiscard]] auto Scancode() const noexcept { return m_Scancode; }
		/**
		 * @brief Returns the mods.
		 *
		 * @return The mods.
		 */
		[[nodiscard]] auto Mods()     const noexcept { return m_Mods; }

	private:
		Input::Key m_Key;
		int        m_Scancode;
		Input::Mod m_Mods;
	};

	/*
	 * @brief The KeyPressed class represents a key pressed event.
	 */
	class KeyPressed : public Key
	{
	public:
		GAZE_REGISTER_EVENT(KeyPressed);

	public:
		KeyPressed(Input::Key key, int scancode, Input::Mod mods) noexcept
			: Key(key, scancode, mods)
		{}
	};

	/*
	 * @brief The KeyRepeat class represents a key repeat event.
	 *
	 * This event is triggered when a key is held down.
	 */
	class KeyRepeat : public Key
	{
	public:
		GAZE_REGISTER_EVENT(KeyRepeat);

	public:
		KeyRepeat(Input::Key key, int scancode, Input::Mod mods) noexcept
			: Key(key, scancode, mods)
		{}
	};

	/*
	 * @brief The KeyReleased class represents a key released event.
	 */
	class KeyReleased : public Key
	{
	public:
		GAZE_REGISTER_EVENT(KeyReleased);

	public:
		KeyReleased(Input::Key key, int scancode, Input::Mod mods) noexcept
			: Key(key, scancode, mods)
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
	inline auto ToString(const Events::Key& event) -> std::string
	{
		return std::format(
			"{}\n"
			"  - Type: Key\n"
			"  - Keycode: {} ({})\n"
			"  - Scancode: {}\n"
			"  - Mods  :\n"
			"    - kModShift    : {}\n"
			"    - kModControl  : {}\n"
			"    - kModAlt      : {}\n"
			"    - kModSuper    : {}\n"
			"    - kModCapsLock : {}\n"
			"    - kModNumLock  : {}",
			ToString(static_cast<Events::Event>(event)),
			int(event.Keycode()), char(event.Keycode()),
			event.Scancode(),
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
	inline auto ToString(const Events::KeyPressed& event) -> std::string
	{
		return std::format("{}\n  - State: Pressed", ToString(static_cast<Events::Key>(event)));
	}

	/**
	 * @brief Returns the string representation of the event.
	 *
	 * @param event The event.
	 *
	 * @return The string representation of the event.
	 */
	[[nodiscard]]
	inline auto ToString(const Events::KeyRepeat& event) -> std::string
	{
		return std::format("{}\n  - State: Repeat", ToString(static_cast<Events::Key>(event)));
	}

	/**
	 * @brief Returns the string representation of the event.
	 *
	 * @param event The event.
	 *
	 * @return The string representation of the event.
	 */
	[[nodiscard]]
	inline auto ToString(const Events::KeyReleased& event) -> std::string
	{
		return std::format("{}\n  - State: Released", ToString(static_cast<Events::Key>(event)));
	}
}

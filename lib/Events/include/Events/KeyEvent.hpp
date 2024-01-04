#pragma once

#include "Event.hpp"

#include "Input/KeyCode.hpp"

namespace Gaze::Events {
	class Key : public Event
	{
	public:
		Key(Input::Key key, int scancode, Input::Mod mods) noexcept
			: m_Key(key)
			, m_Scancode(scancode)
			, m_Mods(mods)
		{}

		[[nodiscard]]
		auto Keycode()  const noexcept { return m_Key; }
		[[nodiscard]]
		auto Scancode() const noexcept { return m_Scancode; }
		[[nodiscard]]
		auto Mods()     const noexcept { return m_Mods; }

	private:
		Input::Key m_Key;
		int m_Scancode;
		Input::Mod m_Mods;
	};

	class KeyPressed : public Key
	{
	public:
		GAZE_REGISTER_EVENT(KeyPressed);

	public:
		KeyPressed(Input::Key key, int scancode, Input::Mod mods) noexcept
			: Key(key, scancode, mods)
		{}
	};

	class KeyRepeat : public Key
	{
	public:
		GAZE_REGISTER_EVENT(KeyRepeat);

	public:
		KeyRepeat(Input::Key key, int scancode, Input::Mod mods) noexcept
			: Key(key, scancode, mods)
		{}
	};

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

	[[nodiscard]]
	inline auto ToString(const Events::KeyPressed& event) -> std::string
	{
		return std::format("{}\n  - State: Pressed", ToString(static_cast<Events::Key>(event)));
	}

	[[nodiscard]]
	inline auto ToString(const Events::KeyRepeat& event) -> std::string
	{
		return std::format("{}\n  - State: Repeat", ToString(static_cast<Events::Key>(event)));
	}

	[[nodiscard]]
	inline auto ToString(const Events::KeyReleased& event) -> std::string
	{
		return std::format("{}\n  - State: Released", ToString(static_cast<Events::Key>(event)));
	}
}

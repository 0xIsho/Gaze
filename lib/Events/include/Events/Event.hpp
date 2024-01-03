#pragma once

#include <string>
#include <format>

namespace Gaze::Events
{
#define GAZE_REGISTER_EVENT(type) \
	static constexpr auto kEventType = Type::type; \
	auto GetType() const -> Type override { return Type::type; }

	class Event
	{
	public:
		enum class Type
		{
			Generic,
			WindowClose,
			WindowFocus,

			MouseMove,
			MouseButtonPressed,
			MouseButtonReleased,
		};

		static constexpr auto kEventType = Type::Generic;

	public:
		virtual ~Event() = default;

		[[nodiscard]]
		auto Handled() const -> bool { return m_Handled; }
		auto SetHandled(bool value) -> void { m_Handled = value; }

		virtual auto GetType() const -> Type
		{
			return Type::Generic;
		}

	private:
		bool m_Handled = false;
	};
}

namespace Gaze {
	[[nodiscard]]
	inline auto ToString(Events::Event::Type type) -> std::string
	{
		switch (type) {
		case Events::Event::Type::Generic:             return "Generic";
		case Events::Event::Type::WindowClose:         return "WindowClose";
		case Events::Event::Type::WindowFocus:         return "WindowFocus";
		case Events::Event::Type::MouseMove:           return "MouseMove";
		case Events::Event::Type::MouseButtonPressed:  return "MouseButtonPressed";
		case Events::Event::Type::MouseButtonReleased: return "MouseButtonReleased";
		}

		return "Unknown";
	}

	[[nodiscard]]
	inline auto ToString(const Events::Event& event) -> std::string
	{
		return std::format(
			"Event:\n"
			"  - Handled: {}",
			event.Handled() ? "True" : "False"
		);
	}
}
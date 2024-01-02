#pragma once

namespace Gaze::Events
{
#define GAZE_REGISTER_EVENT(type) \
	static constexpr auto kEventType = Type::type; \
	auto GetType() -> Type override { return Type::type; }

	class Event
	{
	public:
		enum class Type
		{
			Generic,
			WindowClose,
			WindowFocus,

			MouseMove,
		};

		static constexpr auto kEventType = Type::Generic;

	public:
		virtual ~Event() = default;

		[[nodiscard]]
		auto Handled() const -> bool { return m_Handled; }
		auto SetHandled(bool value) -> void { m_Handled = value; }

		virtual auto GetType() -> Type
		{
			return Type::Generic;
		}

	private:
		bool m_Handled = false;
	};
}

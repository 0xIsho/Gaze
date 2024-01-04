#pragma once

#include "Events/Event.hpp"

#include <string>
#include <functional>
#include <string_view>

struct GLFWwindow;

namespace Gaze::WM {
	class Window
	{
	public:
		using EventCallback = std::function<void(Events::Event&)>;

	public:
		Window(std::string_view title, int width, int height);
		Window(const Window&) = delete;
		Window(Window&& other)                     noexcept;
		~Window();

		auto operator=(const Window&) -> Window& = delete;
		auto operator=(Window&& other)             noexcept -> Window&;

		auto Show()                                noexcept -> void;
		auto Hide()                                noexcept -> void;
		auto Close()                               noexcept -> void;

		auto OnEvent(EventCallback callback)       noexcept -> void;

		[[nodiscard]] auto Width()           const noexcept -> int;
		[[nodiscard]] auto Height()          const noexcept -> int;
		[[nodiscard]] auto Handle()          const noexcept -> void*;

	private:
		GLFWwindow* m_Handle = nullptr;
		std::string m_Title;
		int m_Width = 0;
		int m_Height = 0;

		EventCallback m_CbEvent = [] (auto&) { };
	};

	inline auto Window::Width() const noexcept -> int
	{
		return m_Width;
	}

	inline auto Window::Height() const noexcept-> int
	{
		return m_Height;
	}

	inline auto Window::Handle() const noexcept -> void*
	{
		return reinterpret_cast<void*>(m_Handle);
	}

	inline auto Window::OnEvent(EventCallback callback) noexcept -> void
	{
		m_CbEvent = std::move(callback);
	}
}

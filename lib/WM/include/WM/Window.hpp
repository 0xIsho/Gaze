#pragma once

#include "Events/Event.hpp"

#include <string>
#include <functional>
#include <string_view>

struct GLFWwindow;

namespace Gaze::WM {
	/**
	 * @brief Represents an Operating System window
	 */
	class Window
	{
	public:
		using EventCallback = std::function<void(Events::Event&)>;

	public:
		/**
		 * Construct a new window object
		 *
		 * The Window Manager must be initialised, through a successful call to
		 * @ref WM::Init(), before a Window can be created.
		 *
		 * The new Window is hidden by default, it can be made visible through a
		 * call to @ref Window::Show().
		 *
		 * @param title The window title
		 * @param width The window width, must be > 0
		 * @param height The window height, must be > 0
		 */
		Window(std::string_view title, int width, int height);
		Window(const Window&) = delete;

		/**
		 * @brief Move constructor
		 */
		Window(Window&& other)                     noexcept;

		/**
		 * @brief Destructor - Has a similar effect to @ref Window::Close()
		 */
		~Window();

		auto operator=(const Window&) -> Window& = delete;

		/**
		 * @brief Move assignment operator
		 */
		auto operator=(Window&& other)             noexcept -> Window&;

		/**
		 * @brief Makes the Window visible
		 *
		 * @note Windows are hidden by default
		 */
		auto Show()                                noexcept -> void;

		/**
		 * @brief Hides the Window
		 */
		auto Hide()                                noexcept -> void;

		/**
		 * @brief Close the Window and release any associated resources
		 *
		 * After calling this function, the Window object does *not* refer to a
		 * valid system window object and must be discarded.
		 */
		auto Close()                               noexcept -> void;

		/**
		 * @brief Register a callback to be called when events occur
		 *
		 * @see WM::PollEvents()
		 */
		auto OnEvent(EventCallback callback)       noexcept -> void;

		/**
		 * @brief Retrieve the Window's width
		 */
		[[nodiscard]] auto Width()           const noexcept -> int;

		/**
		 * @brief Retrieve the Window's height
		 */
		[[nodiscard]] auto Height()          const noexcept -> int;

		/**
		 * @brief Retrieve the underlying, platform-specific window handle
		 */
		[[nodiscard]] auto Handle()          const noexcept -> void*;

	private:
		GLFWwindow* m_Handle = nullptr;
		int         m_Width = 0;
		int         m_Height = 0;

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

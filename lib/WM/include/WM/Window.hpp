#pragma once

#include <string>
#include <functional>
#include <string_view>

struct GLFWwindow;

namespace Gaze::WM {
	class Window
	{
	public:
		using CloseCallback = std::function<void()>;
		using MouseMoveCallback = std::function<void(double, double)>;

	public:
		Window(std::string_view title, int width, int height);
		Window(const Window&) = delete;
		Window(Window&& other) noexcept;
		~Window();

		auto operator=(const Window&) -> Window& = delete;
		auto operator=(Window&& other) noexcept -> Window&;

		auto Show() -> void;
		auto Hide() -> void;
		auto Close() -> void;

		auto OnClose(CloseCallback callback) -> void;
		auto OnMouseMove(MouseMoveCallback callback) -> void;

		[[nodiscard]] auto Width() const -> int;
		[[nodiscard]] auto Height() const -> int;
		[[nodiscard]] auto Handle() const -> void*;

	private:
		GLFWwindow* m_Handle = nullptr;
		std::string m_Title;
		int m_Width = 0;
		int m_Height = 0;

		CloseCallback m_CbClose;
		MouseMoveCallback m_CbMouseMove;
	};

	inline auto Window::Width() const -> int
	{
		return m_Width;
	}

	inline auto Window::Height() const -> int
	{
		return m_Height;
	}

	inline auto Window::Handle() const -> void*
	{
		return reinterpret_cast<void*>(m_Handle);
	}
}

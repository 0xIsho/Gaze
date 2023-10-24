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

	public:
		Window(std::string_view title, int width, int height);
		Window(const Window&) = delete;
		Window(Window&& other) noexcept;
		~Window();

		auto operator=(const Window&) -> Window& = delete;
		auto operator=(Window&& other) noexcept -> Window&;

		[[nodiscard]]
		auto Show() -> bool;
		auto Hide() -> void;
		auto Close() -> void;

		auto SwapBuffers() -> void;

		auto OnClose(CloseCallback callback) -> void;

	private:
		GLFWwindow* m_Handle = nullptr;
		std::string m_Title;
		int m_Width = 0;
		int m_Height = 0;

		CloseCallback m_CbClose;
	};
}

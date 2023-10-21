#include "WM/Window.hpp"
#include "WM/Core.hpp"

#include "Debug/Assert.hpp"

#include <GLFW/glfw3.h>

#include <utility>

namespace Gaze::WM {
	Window::Window(std::string_view title, int width, int height)
		: m_Title(title.empty() ? "Gaze" : title)
		, m_Width(width <= 0 ? 800 : width)
		, m_Height(height <= 0 ? 600 : height)
	{
		GAZE_ASSERT(!title.empty(), "Title was empty.");
		GAZE_ASSERT(width > 0, "Window width must be positive.");
		GAZE_ASSERT(height > 0, "Window height must be positive.");
	}

	Window::~Window()
	{
		if (m_Handle) {
			glfwDestroyWindow(m_Handle);
			m_Handle = nullptr;
		}
	}

	Window::Window(Window&& other) noexcept
		: m_Handle(std::exchange(other.m_Handle, nullptr))
		, m_Title(std::exchange(other.m_Title, std::string()))
		, m_Width(std::exchange(other.m_Width, 0))
		, m_Height(std::exchange(other.m_Height, 0))
	{
	}

	auto Window::operator=(Window&& other) noexcept -> Window&
	{
		if (this != &other) {
			std::swap(m_Handle, other.m_Handle);
			std::swap(m_Title, other.m_Title);
			std::swap(m_Width, other.m_Width);
			std::swap(m_Height, other.m_Height);
		}

		return *this;
	}

	auto Window::Show() -> bool
	{
		if (m_Handle == nullptr) {
			GAZE_ASSERT(WM::IsInitialized(), "The Window Manager must be initialized before a window can be created.");
			m_Handle = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
			if (m_Handle == nullptr) {
				return false;
			}
			glfwSetWindowUserPointer(m_Handle, reinterpret_cast<void*>(this));
			glfwSetWindowCloseCallback(m_Handle, [](GLFWwindow* win) {
				const auto* self = static_cast<Window*>(glfwGetWindowUserPointer(win));

				if (self->m_CbClose) {
					self->m_CbClose();
				}
			});
		}

		glfwShowWindow(m_Handle);

		return true;
	}

	auto Window::Hide() -> void
	{
		GAZE_ASSERT(m_Handle != nullptr, "Window::Hide() called with a null handle. Call Window::Show() first.");

		if (m_Handle) {
			glfwHideWindow(m_Handle);
		}
	}

	auto Window::Close() -> void
	{
		GAZE_ASSERT(m_Handle != nullptr, "Window::Close() called with a null handle. Call Window::Show() first.");

		if (m_Handle) {
			glfwDestroyWindow(m_Handle);
		}
	}

	auto Window::Update() -> void
	{
		glfwSwapBuffers(m_Handle);
	}

	auto Window::OnClose(CloseCallback callback) -> void
	{
		m_CbClose = std::move(callback);
	}
}

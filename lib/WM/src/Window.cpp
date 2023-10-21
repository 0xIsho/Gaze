#include "WM/Window.hpp"
#include "WM/Core.hpp"

#include "Debug/Assert.hpp"

#include <GLFW/glfw3.h>

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

	auto Window::Show() -> bool
	{
		if (m_Handle == nullptr) {
			GAZE_ASSERT(WM::IsInitialized(), "The Window Manager must be initialized before a window can be created.");
			m_Handle = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
			if (m_Handle == nullptr) {
				return false;
			}
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

	auto Window::ShouldClose() -> bool
	{
		return glfwWindowShouldClose(m_Handle);
	}
}

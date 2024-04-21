#include "WM/Window.hpp"
#include "WM/Core.hpp"

#include "Debug/Assert.hpp"

#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Events/WindowEvent.hpp"

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <utility>

namespace Gaze::WM {
	Window::Window(std::string_view title, int width, int height)
		: m_Width(width <= 0 ? 800 : width)
		, m_Height(height <= 0 ? 600 : height)
	{
		GAZE_ASSERT(!title.empty(), "Title was empty.");
		GAZE_ASSERT(width > 0, "Window width must be positive.");
		GAZE_ASSERT(height > 0, "Window height must be positive.");
		GAZE_ASSERT(WM::IsInitialized(), "The Window Manager must be initialized before a window can be created.");

		m_Handle = glfwCreateWindow(m_Width, m_Height, title.empty() ? "Gaze" : title.data(), nullptr, nullptr);
		if (m_Handle == nullptr) {
			throw std::runtime_error("Failed to create a window.");
		}

		// Enable Num/CapsLock state reporting in input callbacks
		glfwSetInputMode(m_Handle, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

		glfwSetWindowUserPointer(m_Handle, reinterpret_cast<void*>(this));
		glfwSetWindowCloseCallback(m_Handle, [](GLFWwindow* win) {
			const auto* self = static_cast<Window*>(glfwGetWindowUserPointer(win));
			auto event = Events::WindowClose();
			self->m_CbEvent(event);
		});

		glfwSetWindowFocusCallback(m_Handle, [](GLFWwindow* win, int focused) {
			const auto* self = static_cast<Window*>(glfwGetWindowUserPointer(win));
			auto event = Events::WindowFocus(focused == GLFW_TRUE ? true : false);
			self->m_CbEvent(event);
		});
		glfwSetCursorPosCallback(m_Handle, [](auto* window, auto x, auto y) {
			const auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

			auto event = Events::MouseMove(x, y);
			self->m_CbEvent(event);
		});
		glfwSetMouseButtonCallback(m_Handle, [](auto* window, auto button, auto action, auto mods) {
			const auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

			if (action == GLFW_PRESS) {
				auto event = Events::MouseButtonPressed(Input::MouseButton(button), Input::Mod(mods));
				self->m_CbEvent(event);
			} else if (action == GLFW_RELEASE) {
				auto event = Events::MouseButtonReleased(Input::MouseButton(button), Input::Mod(mods));
				self->m_CbEvent(event);
			} else {
				// TODO: More graceful handling
				GAZE_ASSERT(false, "Unsupported mouse button event.");
			}
		});
		glfwSetKeyCallback(m_Handle, [](auto* window, auto keycode, auto scancode, auto action, auto mods) {
			const auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

			if(action == GLFW_PRESS) {
				auto event = Events::KeyPressed(Input::Key(keycode), scancode, Input::Mod(mods));
				self->m_CbEvent(event);
			} else if (action == GLFW_REPEAT) {
				auto event = Events::KeyRepeat(Input::Key(keycode), scancode, Input::Mod(mods));
				self->m_CbEvent(event);
			} else if (action == GLFW_RELEASE) {
				auto event = Events::KeyReleased(Input::Key(keycode), scancode, Input::Mod(mods));
				self->m_CbEvent(event);
			} else {
				// TODO: More graceful handling
				GAZE_ASSERT(false, "Unsupported key event.");
			}
		});
	}

	Window::~Window()
	{
		Close();
	}

	Window::Window(Window&& other) noexcept
		: m_Handle(std::exchange(other.m_Handle, nullptr))
		, m_Width(std::exchange(other.m_Width, 0))
		, m_Height(std::exchange(other.m_Height, 0))
	{
	}

	auto Window::operator=(Window&& other) noexcept -> Window&
	{
		if (this != &other) {
			std::swap(m_Handle, other.m_Handle);
			std::swap(m_Width, other.m_Width);
			std::swap(m_Height, other.m_Height);
		}

		return *this;
	}

	auto Window::Show() noexcept -> void
	{
		glfwShowWindow(m_Handle);
	}

	auto Window::Hide() noexcept -> void
	{
		GAZE_ASSERT(m_Handle != nullptr, "Window::Hide() called with a null handle. Call Window::Show() first.");

		if (m_Handle) {
			glfwHideWindow(m_Handle);
		}
	}

	auto Window::Close() noexcept -> void
	{
		GAZE_ASSERT(m_Handle != nullptr, "Window::Close() called with a null handle. Call Window::Show() first.");

		if (m_Handle) {
			glfwDestroyWindow(m_Handle);
			m_Handle = nullptr;
		}
	}
}

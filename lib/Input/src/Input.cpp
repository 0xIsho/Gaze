#include "Input/Input.hpp"

#include "WM/Window.hpp"

#include <GLFW/glfw3.h>

namespace Gaze::Input {
	Handler::Handler(Mem::Shared<WM::Window> win)
		: m_Win(std::move(win))
	{
	}

	auto Handler::IsKeyPressed(Key key) const -> bool
	{
		return glfwGetKey(static_cast<GLFWwindow*>(m_Win->Handle()), static_cast<int>(key)) == GLFW_PRESS;
	}
}

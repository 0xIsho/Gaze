#include "WM/Core.hpp"

#include <GLFW/glfw3.h>

namespace Gaze::WM {
	static auto g_IsInitialized = false;

	auto Init() -> bool
	{
		return g_IsInitialized = (glfwInit() == GLFW_TRUE);
	}

	auto Terminate() -> void
	{
		glfwTerminate();
		g_IsInitialized = false;
	}

	auto IsInitialized() -> bool
	{
		return g_IsInitialized;
	}

	auto PollEvents() -> void
	{
		glfwPollEvents();
	}
}

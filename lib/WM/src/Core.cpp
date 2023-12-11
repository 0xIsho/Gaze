#include "WM/Core.hpp"

#include <GLFW/glfw3.h>

#include <iostream>

namespace Gaze::WM {
	static auto g_IsInitialized = false;

	auto Init() -> bool
	{
		g_IsInitialized = glfwInit() == GLFW_TRUE;

		if (g_IsInitialized) {
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // TODO: This should be ignored when using OpenGL
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // TODO: Remove this once resizing logic is implemented.

			glfwSetErrorCallback([](int code, const char* description) {
				std::cerr << "GLFW Error (" << code << "): " << description << '\n';
			});
		}

		return g_IsInitialized;
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

#include "WM/Core.hpp"

#include <GLFW/glfw3.h>

#include <iostream>

namespace Gaze::WM {
	static auto s_IsInitialized = false;

	auto Init() -> bool
	{
		s_IsInitialized = glfwInit() == GLFW_TRUE;

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // TODO: This should be ignored when using OpenGL
		if (s_IsInitialized) {
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // TODO: Remove this once resizing logic is implemented.

			glfwSetErrorCallback([](int code, const char* description) {
				std::cerr << "GLFW Error (" << code << "): " << description << '\n';
			});
		}

		return s_IsInitialized;
	}

	auto Terminate() -> void
	{
		glfwTerminate();
		s_IsInitialized = false;
	}

	auto IsInitialized() -> bool
	{
		return s_IsInitialized;
	}

	auto PollEvents() -> void
	{
		glfwPollEvents();
	}
}

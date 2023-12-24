#include "WM/Core.hpp"

#include <GLFW/glfw3.h>

#include <iostream>

namespace Gaze::WM {
	static auto s_IsInitialized = false;

	auto Init() -> bool
	{
		s_IsInitialized = glfwInit() == GLFW_TRUE;

		if (s_IsInitialized) {
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // TODO: Remove this once resizing logic is implemented.

			glfwSetErrorCallback([](int code, const char* description) {
				std::cerr << "GLFW Error (" << code << "): " << description << '\n';
			});

#ifndef NDEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
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

#include "Client/App.hpp"

#include <iostream>

namespace Gaze::Client {
	App::App(int /*argc*/, char** /*argv*/)
	{
	}

	auto App::Run() -> Status
	{
		std::cout << "Hello, World!\n";
		return Status::Success;
	}
}

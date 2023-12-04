#include "Client/App.hpp"

#include "Debug/Assert.hpp"

#include "WM/Core.hpp"

namespace Gaze::Client {
	App::App(int /*argc*/, char** /*argv*/)
	{
		GAZE_ASSERT(m_IsRunning == false, "The application should not be running yet.");
	}

	App::~App()
	{
		m_IsRunning = false;
	}

	auto App::Run() -> Status
	{
		if (OnInit() == Status::Success) {
			m_IsRunning = true;
		}

		while (m_IsRunning) {
			OnUpdate();
			Gaze::WM::PollEvents();
		}

		return OnShutdown();
	}

	auto App::Quit() -> void
	{
		m_IsRunning = false;
	}
}

auto main(int argc, char** argv) -> int
{
	auto ret = 1;
	if (Gaze::WM::Init()) {
		auto app = Gaze::Client::CreateApp(argc, argv);
		ret = app->Run() == Gaze::Client::App::Status::Success ? 0 : 1;
	}
	Gaze::WM::Terminate();
	return ret;
}

#include "Client/App.hpp"

#include "Debug/Assert.hpp"

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
		}

		return OnShutdown();
	}

	auto App::Quit() -> void
	{
		m_IsRunning = false;
	}
}

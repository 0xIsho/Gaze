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

		GAZE_ASSERT(m_IsRunning == false, "The application is still running at the end of the destructor. It should not be!");
	}

	auto App::Run() -> Status
	{
		if (const auto status = OnInit(); status != Status::Success) {
			return status;
		}

		m_IsRunning = true;

		GAZE_ASSERT(m_IsRunning == true, "The application is not running upon reaching the main loop.");
		while (m_IsRunning) {
			OnUpdate();

			static int shutdownCounter = 10; // Run # cycles then break out (we have no other way, for now).
			if (--shutdownCounter <= 0) {
				break;
			}
		}

		const auto ret = OnShutdown();

		m_IsRunning = false;

		GAZE_ASSERT(m_IsRunning == false, "The application should not be running at this point.");
		return ret;
	}
}

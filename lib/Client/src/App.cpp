#include "Client/App.hpp"

namespace Gaze::Client {
	App::App(int /*argc*/, char** /*argv*/)
	{
	}

	App::~App()
	{
		m_IsRunning = false;
	}

	auto App::Run() -> Status
	{
		if (const auto status = OnInit(); status != Status::Success) {
			return status;
		}

		m_IsRunning = true;

		while (m_IsRunning) {
			OnUpdate();

			static int shutdownCounter = 10; // Run # cycles then break out (we have no other way, for now).
			if (--shutdownCounter <= 0) {
				break;
			}
		}

		const auto ret = OnShutdown();

		m_IsRunning = false;

		return ret;
	}
}

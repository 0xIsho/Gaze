#include "Client/App.hpp"

#include "Debug/Assert.hpp"

#include "WM/Core.hpp"

#include <thread>
#include <chrono>

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
		using namespace std::chrono;

		if (OnInit() == Status::Success) {
			m_IsRunning = true;
		}

		auto deltaTime = 1.0 / 30.0;
		auto frameBegin = steady_clock::now();

		while (m_IsRunning) {
			OnUpdate(deltaTime);
			Gaze::WM::PollEvents();

			const auto frameEnd = steady_clock::now();
			deltaTime = F64((frameEnd - frameBegin).count()) / 1'000'000'000;

			// TODO: Temporary solution to the X server getting overloaded at very high framerates
			if (deltaTime < 0.001) {
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}

			// Delta too large. Assume that a debugger took over and paused
			// the engine's execution.
			if (deltaTime > 5.0) {
				deltaTime = 1.0F / 30.0F;
			}

			frameBegin = frameEnd;
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

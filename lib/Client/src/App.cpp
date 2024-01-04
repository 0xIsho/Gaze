#include "Client/App.hpp"

#include "Debug/Assert.hpp"

#include "WM/Core.hpp"

#include <thread>
#include <chrono>

namespace Gaze::Client {
	App::App(int /*argc*/, char** /*argv*/) noexcept
	{
		GAZE_ASSERT(m_IsRunning == false, "The application should not be running yet.");
	}

	App::~App()
	{
		m_IsRunning = false;
	}

	auto App::Run() noexcept -> Status
	try {
		using namespace std::chrono;

		if (OnInit() == Status::Success) {
			m_IsRunning = true;
		}

		auto deltaTime = 1.0 / 30.0;
		auto frameBegin = steady_clock::now();

		constexpr auto fixedTimeStep = 1 / 60.0;
		auto accumulatedTimestep = 0.0;

		while (m_IsRunning) {
			OnUpdate(deltaTime);
			Gaze::WM::PollEvents();

			while (accumulatedTimestep >= fixedTimeStep) {
				OnFixedUpdate(fixedTimeStep);
				accumulatedTimestep -= fixedTimeStep;
			}

			const auto frameEnd = steady_clock::now();
			deltaTime = F64((frameEnd - frameBegin).count()) / 1'000'000'000;

			// Delta too large. Assume that a debugger took over and paused
			// the engine's execution.
			if (deltaTime > 5.0) {
				deltaTime = 1.0F / 30.0F;
			}

			accumulatedTimestep += deltaTime;
			frameBegin = frameEnd;
		}

		return OnShutdown();
	} catch (...) {
		return Status::Fail;
	}

	auto App::Quit() noexcept -> void
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

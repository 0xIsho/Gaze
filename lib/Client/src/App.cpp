#include "Client/App.hpp"

#include "Debug/Assert.hpp"

#include "WM/Core.hpp"

#include "Net/Core.hpp"


#include "Net/Packet.hpp"

#include <thread>
#include <chrono>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace Gaze::Client {
	App::App(int /*argc*/, char** /*argv*/)
	{
		GAZE_ASSERT(m_IsRunning == false, "The application should not be running yet.");

		if (!Gaze::Net::Init()) {
			throw std::runtime_error("Failed to initialize the Network sub-system.");
		}
	}

	App::~App()
	{
		if (Net::IsInitialized()) {
			Net::Terminate();
		}

		m_IsRunning = false;
	}

	auto App::Start() noexcept -> Status
	try {
		if (m_IsRunning) {
			return Status::Success;
		}

		if (OnInit() == Status::Success) {
			m_IsRunning = true;
		} else {
			return Status::Fail;
		}

		Run();

		return OnShutdown();
	} catch (...) {
		return Status::Fail;
	}

	auto App::Quit() noexcept -> void
	{
		m_IsRunning = false;
	}

	ClientApp::ClientApp(int argc, char** argv)
		: App(argc, argv)
	{
		if (!Gaze::WM::Init()) {
			throw std::runtime_error("Failed to initialize the window manager.");
		}
			}

	ClientApp::~ClientApp()
	{
		m_Client.Disconnect();

		if (Gaze::WM::IsInitialized()) {
			Gaze::WM::Terminate();
		}
	}

	auto ClientApp::Run() -> void
	{
		using namespace std::chrono;

		auto timestep = 1.0 / 30.0;
		auto frameBegin = steady_clock::now();

		constexpr auto fixedTimeStep = 1 / 60.0;
		auto accumulatedTimestep = 0.0;

		auto connThread = std::jthread([this] {
			constexpr auto maxRetries = 5;
			auto currentTry = 0;

			while (!m_Client.Connect("127.0.0.1", 54321) && currentTry++ < maxRetries) {
				std::cerr << "Server connection failed. Retrying " << currentTry << '/' << maxRetries << '\n';
			}
		});

		m_Client.OnPacketReceived([this](auto senderID, auto packet) {
			OnPacketReceived(senderID, std::move(packet));
		});

		while (m_IsRunning) {
			OnUpdate(timestep);
			Gaze::WM::PollEvents();
			m_Client.Update();

			while (accumulatedTimestep >= fixedTimeStep) {
				OnFixedUpdate(fixedTimeStep);
				accumulatedTimestep -= fixedTimeStep;
			}

			const auto frameEnd = steady_clock::now();
			timestep = F64((frameEnd - frameBegin).count()) / 1'000'000'000;

			// Delta too large. Assume that a debugger took over and paused
			// the engine's execution.
			if (timestep > 5.0) {
				timestep = 1.0 / 30.0;
			}

			accumulatedTimestep += timestep;
			frameBegin = frameEnd;
		}
	}

	auto ClientApp::Send(Net::Packet packet, U8 channel /*= 0*/) -> bool
	{
		return m_Client.Send(std::move(packet), channel);
	}

	ServerApp::ServerApp(int argc, char** argv)
		: App(argc, argv)
	{
	}

	ServerApp::~ServerApp()
	{
	}

	auto ServerApp::Run() -> void
	{
		using namespace std::chrono;

		m_Server.OnPacketReceived([this](auto senderID, auto packet) {
			OnPacketReceived(senderID, std::move(packet));
		});
		m_Server.OnClientConnected([this](auto clientID) {
			OnClientConnected(clientID);
		});

		auto timestep = 1.0 / 30.0;
		auto frameBegin = steady_clock::now();

		constexpr auto fixedTimeStep = 1 / 60.0;
		auto accumulatedTimestep = 0.0;

		while (m_IsRunning) {
			m_Server.Update();
			OnUpdate(timestep);

			while (accumulatedTimestep >= fixedTimeStep) {
				OnFixedUpdate(fixedTimeStep);
				accumulatedTimestep -= fixedTimeStep;
			}

			const auto frameEnd = steady_clock::now();
			timestep = F64((frameEnd - frameBegin).count()) / 1'000'000'000;

			accumulatedTimestep += timestep;
			frameBegin = frameEnd;
		}
	}

	auto ServerApp::Send(U32 peerID, Net::Packet packet, U8 channel /*= 0*/) -> bool
	{
		return m_Server.Send(peerID, std::move(packet), channel);
	}

	auto ServerApp::Broadcast(Net::Packet packet, U8 channel /*= 0*/) -> void
	{
		m_Server.Broadcast(std::move(packet), channel);
	}
}

auto main(int argc, char** argv) -> int
{
	auto app = Gaze::Client::CreateApp(argc, argv);
	return app->Start() == Gaze::Client::App::Status::Success ? 0 : 1;
}

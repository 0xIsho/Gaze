#include "Client/App.hpp"

#include "Debug/Assert.hpp"

#include "WM/Core.hpp"

#include "Net/Core.hpp"
#include "Net/Packet.hpp"

#include "Log/Logger.hpp"

#include "Config/Parser.hpp"

#include <thread>
#include <chrono>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <filesystem>

namespace Gaze::Client {
	App::App(int /*argc*/, char** /*argv*/)
	{
		GAZE_ASSERT(m_IsRunning == false, "The application should not be running yet.");

		Log::Logger::LogsDirectoryPath("Engine/Logs/");

		static const auto configDirPath = [] { return std::filesystem::path("Engine/Config/").make_preferred(); }();
		if (std::filesystem::exists(configDirPath)) {
			Config::Parser::ConfigDirectoryPath(configDirPath);

			using DirIt = std::filesystem::recursive_directory_iterator;
			auto parser = Config::Parser(&m_Config);

			for (const auto& entry : DirIt(configDirPath)) {
				if (!(entry.is_regular_file() && entry.path().extension() == ".conf")) {
					continue;
				}

				const auto relativeToConfigDir = std::filesystem::relative(entry.path(), configDirPath);
				if (!parser.Load(relativeToConfigDir)) {
					std::cerr << "Error loading config file: " << configDirPath / relativeToConfigDir << '\n';
				}
			}
		}

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
		if (m_Client.IsConnected()) {
		m_Client.Disconnect();
		}

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

			auto timeout = 3000;
			if (!m_Config.Get<int>("/Engine/Net/Client", "ConnectionAttemptTimeout", timeout)) {
				std::cerr << "Error loading '/Engine/Net/Client.ConnectionAttemptTimeout'"
					"configuration option. Using default value: " << timeout << '\n';
			}
			else {
				std::cout << "/Engine/Net/Client.ConnectionAttemptTimeout: " << timeout << '\n';
			}

			auto address = std::string("127.0.0.1");
			if (!m_Config.Get<std::string>("/Engine/Net/Client", "ServerAddress", address)) {
				std::cerr << "Error loading '/Engine/Net/Client.ServerAddress'"
					"configuration option. Using default value: " << address << '\n';
			}

			while (m_IsRunning && !m_Client.Connect(address, 54321, U32(timeout)) && currentTry++ < maxRetries) {
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
			timestep = std::chrono::duration<double>(frameEnd - frameBegin) / 1s;

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

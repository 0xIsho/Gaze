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
		: m_Logger([] { Log::Logger::LogsDirectoryPath("Engine/Logs/"); return Log::Logger("App"); }())
		, m_ClientLogger("Game")
	{
		GAZE_ASSERT(m_IsRunning == false, "The application should not be running yet.");

		// TODO: Add version info
		m_Logger.Info("Gaze Engine starting...");

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
				m_Logger.Trace("Loading configuration file: {}", (configDirPath / relativeToConfigDir).string());
				if (!parser.Load(relativeToConfigDir)) {
					m_Logger.Error("Error loading configuration file: {}", (configDirPath / relativeToConfigDir).string());
				}

			}
		}
		else {
			m_Logger.Warn("Configuration directory '{}' does not exist.", configDirPath.string());
		}

		m_Logger.Info("Initialising the Networking sub-system...");
		if (!Gaze::Net::Init()) {
			m_Logger.Error("Error initialsing the Network sub-system.");
		}
	}

	App::~App()
	{
		if (Net::IsInitialized()) {
			m_Logger.Info("Terminating the Networking sub-system");
			Net::Terminate();
		}

		m_IsRunning = false;
	}

	auto App::Logger() -> Log::Logger&
	{
		return m_ClientLogger;
	}

	auto App::Start() noexcept -> Status
	try {
		if (m_IsRunning) {
			return Status::Success;
		}

		if (OnInit() == Status::Success) {
			m_IsRunning = true;
		} else {
			m_Logger.Critical("Engine initialisation failed.");
			m_Logger.Info("Shutting down...");
			return Status::Fail;
		}

		m_Logger.Info("Running...");
		Run();

		m_Logger.Info("Shutting down...");
		return OnShutdown();
	}
	catch (const std::exception& ex) {
		m_Logger.Critical("Unhandled exception: {}", ex.what());
		return Status::Fail;
	}
	catch (...) {
		m_Logger.Critical("Unknown unhandled exception.");
		return Status::Fail;
	}

	auto App::Quit() noexcept -> void
	{
		m_Logger.Info("Quit requested.");
		m_IsRunning = false;
	}

	ClientApp::ClientApp(int argc, char** argv)
		: App(argc, argv)
		, m_Logger("AppClient")
	{
		m_Logger.Info("Initialising the Window Manager...");
		if (!Gaze::WM::Init()) {
			m_Logger.Error("Error initialising the Window Manager");
		}
	}

	ClientApp::~ClientApp()
	{
		if (m_Client.IsConnected()) {
			m_Logger.Info("Disconnecting remote peer...");
			m_Client.Disconnect();
		}

		if (Gaze::WM::IsInitialized()) {
			m_Logger.Info("Terminating the Window Manager");
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
			if (m_Config.Get<int>("/Engine/Net/Client", "ConnectionAttemptTimeout", timeout)) {
				m_Logger.Info("/Engine/Net/Client.ConnectionAttemptTimeout: {}", timeout);
			}
			else {
				m_Logger.Error("Error retrieving '/Engine/Net/Client.ConnectionAttemptTimeout'. Using default: {}", timeout);
			}

			auto address = std::string("127.0.0.1");
			if (m_Config.Get<std::string>("/Engine/Net/Client", "ServerAddress", address)) {
				m_Logger.Info("/Engine/Net/Client.ServerAddress: {}", address);
			}
			else {
				m_Logger.Error("Error retrieving '/Engine/Net/Client.ServerAddress'. Using default: {}", address);
			}

			while (m_IsRunning && !m_Client.Connect(address, 54321, U32(timeout)) && currentTry++ < maxRetries) {
				m_Logger.Error("Server connection failed. Retrying {}/{}", currentTry, maxRetries);
			}
		});

		m_Client.OnPacketReceived([this](auto senderID, auto packet) {
			OnPacketReceived(senderID, std::move(packet));
		});

		while (m_IsRunning) {
			m_Client.Update();
			OnUpdate(timestep);

			while (accumulatedTimestep >= fixedTimeStep) {
				OnFixedUpdate(fixedTimeStep);
				accumulatedTimestep -= fixedTimeStep;
			}

			Gaze::WM::PollEvents();

			const auto frameEnd = steady_clock::now();
			timestep = std::chrono::duration<double>(frameEnd - frameBegin) / 1s;

			// Delta too large. Assume that a debugger took over and paused
			// the engine's execution.
			if (timestep > 5.0) {
				m_Logger.Warn("TimestepCap reached!");
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
		, m_Logger("AppServer")
		, m_Server(
			[this] {
				auto addr = std::string("0.0.0.0");
				if (m_Config.Get<std::string>("/Engine/Net/Server", "ListenAddress", addr)) {
					m_Logger.Trace("/Engine/Net/Server.ListenAddress: {}", addr);
				}
				else {
					m_Logger.Error("Error retrieving '/Engine/Net/Server.ListenAddress'. Using default: {}", addr);
				}

				return addr;
			}())
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
			m_Logger.Info("New client connected. ID: {}", clientID);
			OnClientConnected(clientID);
		});
		m_Server.OnClientDisconnected([this](auto clientID) {
			m_Logger.Info("Client disconnected. ID: {}", clientID);
			OnClientDisconnected(clientID);
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

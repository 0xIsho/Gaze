#pragma once

#include "Core/Type.hpp"

#include "Net/Server.hpp"
#include "Net/Client.hpp"
#include "Net/Packet.hpp"

namespace Gaze::Client {
	class App
	{
	public:
		enum class Status
		{
			Success = 0,
			Fail
		};

	public:
		App(int argc, char** argv) noexcept;
		virtual ~App();

		[[nodiscard]] auto Start()           noexcept -> Status;
		[[nodiscard]] auto IsRunning() const noexcept -> bool;
		              auto Quit()            noexcept -> void;

	protected:
		[[nodiscard]] virtual auto Run()                   -> Status = 0;
		[[nodiscard]] virtual auto OnInit()                -> Status = 0;
		              virtual auto OnUpdate(F64 deltaTime) -> void = 0;
		[[nodiscard]] virtual auto OnShutdown()            -> Status = 0;
		              virtual auto OnFixedUpdate(F64 /*deltaTime*/) -> void { }
		virtual auto OnPacketReceived(U32 /*sender*/, Net::Packet /*packet*/) -> void { };

	protected:
		bool m_IsRunning = false;
	};

	class ClientApp : public App
	{
	public:
		ClientApp(int argc, char** argv);
		virtual ~ClientApp();

	protected:
		[[nodiscard]] auto Run() -> Status override;

		auto Send(Net::Packet packet, U8 channel = 0) -> bool;

	private:
		Net::Client m_Client;
	};

	class ServerApp : public App
	{
	public:
		ServerApp(int argc, char** argv);
		virtual ~ServerApp();

	protected:
		[[nodiscard]] auto Run() -> Status override;

		auto Send(U32 peerID, Net::Packet packet, U8 channel = 0) -> bool;
		auto Broadcast(Net::Packet packet, U8 channel = 0) -> void;

		virtual auto OnClientConnected(U32 /*clientID*/) -> void { }

	private:
		Net::Server m_Server;
	};

	auto CreateApp(int argc, char** argv) -> Mem::Unique<App>;
}

// The variable declaration at the end is so that a semicolon is required when
// the MACRO is used. Keeps things consistent.
#define GAZE_REGISTER_APP(TApp)                           \
	auto Gaze::Client::CreateApp(int argc, char** argv)   \
		-> ::Gaze::Mem::Unique<::Gaze::Client::App>       \
	{                                                     \
		return ::Gaze::Mem::MakeUnique<TApp>(argc, argv); \
	} int* _gaze_dummy ## __COUNTER__ = nullptr

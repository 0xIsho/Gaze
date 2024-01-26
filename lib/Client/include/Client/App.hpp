#pragma once

#include "Core/Type.hpp"

#include "Net/Server.hpp"
#include "Net/Client.hpp"
#include "Net/Packet.hpp"

namespace Gaze::Client {
	/**
	 * @brief The main application
	 *
	 * This class is the main entry point for the client application. It
	 * initializes the engine and runs the client application.
	 */
	class App
	{
	public:
		enum class Status
		{
			Success = 0,
			Fail
		};

	public:
		/**
		 * @brief Construct a new App object
		 *
		 * @param argc The number of command line arguments
		 * @param argv The command line arguments
		 */
		App(int argc, char** argv);

		/**
		 * @brief Destroy the App object
		 *
		 * The destructor terminates any subsystems managed by the engine.
		 */
		virtual ~App();

		/**
		 * @brief Start the application
		 *
		 * This function runs any setup steps required and initializes any
		 * required engine subsystems.
		 *
		 * @return Status::Success if the engine was successfuly initilized,
		 *         Status::Fail otherwise.
		 */
		[[nodiscard]] auto Start() noexcept -> Status;

		/**
		 * @brief Check if the application is running
		 *
		 * @return true if it is running, false otherwise
		 */
		[[nodiscard]] auto IsRunning() const noexcept -> bool;

		/**
		 * @brief Terminates the application and cleans up used resources
		 *
		 * Note that this function does not forcibly kill the application. It is
		 * just a request for the application to be terminated. The application
		 * may keep running until it finishes any in-progress processing before
		 * it terminates.
		 */
		auto Quit() noexcept -> void;

	protected:
		/**
		 * @brief Initialisation function
		 *
		 * This function is called after the engine's subsystems have been
		 * successfully initialized. Called once. Use it to initialize any
		 * client specific systems.
		 *
		 * @return Status value representing the result of the initialization
		 *         Returning Status::Success informs the engine that the client
		 *         was successfully initialized and the execution will continue.
		 *         Returning Status::Fail informs the engine that the client
		 *         initialization failed and the engine will terminate.
		 */
		[[nodiscard]] virtual auto OnInit() -> Status = 0;
		/**
		 * @brief The client's main entry point. Called once
		 */
		virtual auto Run() -> void = 0;
		/**
		 * @brief The client's update loop. Called once every frame
		 *
		 * @param timestep The time (in milliseconds) since the last update
		 */
		virtual auto OnUpdate(F64 timestep) -> void = 0;
		/**
		 * @brief The client's shutdown function. Called once
		 *
		 * This function is called before the engine's subsystems are
		 * terminated. Use it to clean up any client specific resources.
		 *
		 * @return Status value representing the result of the shutdown
		 */
		[[nodiscard]] virtual auto OnShutdown() -> Status = 0;

		/**
		 * @brief The client's fixed update loop
		 *
		 * This function is called at a fixed time intervals, mainly used for
		 * physics simulation.
		 *
		 * @param timestep The fixed timestep
		 */
		virtual auto OnFixedUpdate(F64) -> void { }

		/**
		 * @brief Called when a packet is received
		 *
		 * This function is called when a packet is received from the remote
		 * peer.
		 *
		 * @param peerID The ID of the remote peer
		 * @param packet The received packet
		 */
		virtual auto OnPacketReceived(U32, Net::Packet) -> void { };

	protected:
		bool m_IsRunning = false;
	};

	/**
	 * @brief The client application
	 *
	 * This class is the main entry point for the client application. It
	 * initializes the engine and runs the client application.
	 */
	class ClientApp : public App
	{
	public:
		/**
		 * @brief Construct a new ClientApp object
		 */
		ClientApp(int argc, char** argv);

		/**
		 * @brief Destroy the ClientApp object
		 *
		 * The destructor terminates any subsystems managed by the engine.
		 */
		virtual ~ClientApp();

	protected:
		/**
		 * @brief The main entry point for the client application
		 */
		auto Run() -> void override;

		/**
		 * @brief Send a packet to the remote peer
		 *
		 * @param packet The packet to send
		 * @param channel The channel to send the packet on (default 0)
		 *
		 * @return true if the packet was sent successfully, false otherwise.
		 */
		auto Send(Net::Packet packet, U8 channel = 0) -> bool;

	private:
		Net::Client m_Client;
	};

	/**
	 * @brief The server application
	 *
	 * This class is the main entry point for the server application. It
	 * initializes the engine and runs the server application.
	 */
	class ServerApp : public App
	{
	public:
		/**
		 * @brief Construct a new ServerApp object
		 */
		ServerApp(int argc, char** argv);

		/**
		 * @brief Destroy the ServerApp object
		 *
		 * The destructor terminates any subsystems managed by the engine.
		 */
		virtual ~ServerApp();

	protected:
		/**
		 * @brief The main entry point for the server application
		 */
		auto Run() -> void override;

		/**
		 * @brief Send a packet to the remote peer
		 *
		 * @param peerID The ID of the remote peer
		 * @param packet The packet to send
		 * @param channel The channel to send the packet on (default 0)
		 *
		 * @return true if the packet was sent successfully, false otherwise.
		 */
		auto Send(U32 peerID, Net::Packet packet, U8 channel = 0) -> bool;

		/**
		 * @brief Broadcast a packet to all connected peers
		 *
		 * @param packet The packet to broadcast
		 * @param channel The channel to broadcast the packet on (default 0)
		 */
		auto Broadcast(Net::Packet packet, U8 channel = 0) -> void;

		/**
		 * @brief Called when a client connects to the server
		 *
		 * @param clientID The ID of the connected client
		 */
		virtual auto OnClientConnected(U32 /*clientID*/) -> void { }

	private:
		Net::Server m_Server;
	};

	/**
	 * @brief Create an application
	 *
	 * This function creates an application instance based on the specified
	 * type.
	 *
	 * The function is meant to be defined through a call to GAZE_REGISTER_APP()
	 * in the client application's source file to inject the client's custom App
	 * type into the engine.
	 *
	 * To use the function, simply define a class that inherits either from
	 * @ref ClientApp or @ref ServerApp then call GAZE_REGISTER_APP() with the
	 * class name as an argument.
	 *
	 * @param argc The number of command line arguments
	 * @param argv The command line arguments
	 *
	 * @return A unique pointer to the created application
	 */
	auto CreateApp(int argc, char** argv) -> Unique<App>;
}

// The variable declaration at the end is so that a semicolon is required when
// the MACRO is used. Keeps things consistent.
#define GAZE_REGISTER_APP(TApp)                           \
	auto Gaze::Client::CreateApp(int argc, char** argv)   \
		-> ::Gaze::Unique<::Gaze::Client::App>       \
	{                                                     \
		return ::Gaze::MakeUnique<TApp>(argc, argv); \
	} int* _gaze_dummy ## __COUNTER__ = nullptr

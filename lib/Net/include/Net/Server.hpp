#pragma once

#include "Core/Type.hpp"

#include "Net/Type.hpp"
#include "Net/Packet.hpp"

namespace Gaze::Net {
	class Server
	{
		struct Impl;

	public:
		using ClientConnectedCallback = std::function<void(U32)>;

	public:
		Server(U32 host = 0, U16 port = 54321);
		~Server();

		auto Update() -> void;

		auto Broadcast(Packet packet, U8 channel = 0) -> void;
		auto Send(U32 peerID, Packet packet, U8 channel = 0) -> bool;

		auto OnPacketReceived(PacketReceivedCallback callback) -> void;
		auto OnClientConnected(ClientConnectedCallback callback) -> void;
	private:
		Impl* m_pImpl;
	};
}
#include "Net/Server.hpp"

#include "Debug/Assert.hpp"

#include <enet/enet.h>

namespace Gaze::Net {
	struct Server::Impl
	{
		ENetHost* host;

		PacketReceivedCallback cbPacketReceived = [](auto, auto) {};
		ClientConnectedCallback cbClientConnected = [](auto) {};
	};

	Server::Server(U32 host /*= 0*/, U16 port /*= 54321*/)
		: m_pImpl(new Impl())
	{
		auto addr = ENetAddress();
		addr.host = host;
		addr.port = port;

		m_pImpl->host = enet_host_create(&addr, 32, 2, 0, 0);

		// TODO: Proper error handling
		GAZE_ASSERT(m_pImpl->host != nullptr, "Failed to create a server");
	}

	Server::~Server()
	{
		enet_host_destroy(m_pImpl->host);
		delete m_pImpl;
	}

	auto Server::Update() -> void
	{
		enet_host_service(m_pImpl->host, nullptr, 0);
		auto event = ENetEvent();
		while (enet_host_check_events(m_pImpl->host, &event) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_CONNECT:
				printf("New peer connection from %x:%u.\n",
					event.peer->address.host,
					event.peer->address.port
					);
				GAZE_ASSERT(m_pImpl->cbClientConnected, "NULL callback");
				m_pImpl->cbClientConnected(event.peer->incomingPeerID);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				printf("Peer %x:%u disconnected.\n",
					event.peer->address.host,
					event.peer->address.port
					);
				event.peer->data = nullptr;
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				GAZE_ASSERT(m_pImpl->cbPacketReceived, "NULL callback");

				m_pImpl->cbPacketReceived(
					event.peer->incomingPeerID,
					Packet(event.packet->data, event.packet->dataLength)
				);
				enet_packet_destroy(event.packet);
				break;
			}
		}
	}

	auto Server::Broadcast(Packet packet, U8 channel /*= 0*/) -> void
	{
		enet_host_broadcast(m_pImpl->host, channel, static_cast<ENetPacket*>(packet.Handle()));
	}

	auto Server::Send(U32 peerID, Packet packet, U8 channel /*= 0*/) -> bool
	{
		return enet_peer_send(
			&(m_pImpl->host->peers[peerID]),
			channel, static_cast<ENetPacket*>(packet.Handle())
		) == 0;
	}

	auto Server::OnPacketReceived(PacketReceivedCallback callback) -> void
	{
		m_pImpl->cbPacketReceived = std::move(callback);
	}

	auto Server::OnClientConnected(ClientConnectedCallback callback) -> void
	{
		m_pImpl->cbClientConnected = std::move(callback);
	}
}

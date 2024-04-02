#include "Net/Client.hpp"

#include "Debug/Assert.hpp"

#include <enet/enet.h>

namespace Gaze::Net {
	struct Client::Impl
	{
		ENetHost* host;

		PacketReceivedCallback cbPacketReceived = [](auto, auto) {};
	};

	Client::Client()
		: m_pImpl(new Impl())
	{
		m_pImpl->host = enet_host_create(nullptr, 1, 2, 0, 0);

		// TODO: Proper error handling
		GAZE_ASSERT(m_pImpl->host != nullptr, "Failed to create a client");
	}

	Client::~Client()
	{
		enet_host_destroy(m_pImpl->host);
		delete m_pImpl;
	}

	auto Client::Connect(std::string_view host, U16 port) -> bool
	{
		auto addr = ENetAddress();
		if (enet_address_set_host(&addr, host.data()) < 0) {
			return false;
		}
		addr.port = port;

		printf("Attempting connection to %s:%u...\n", host.data(), port);

		auto peer = enet_host_connect(m_pImpl->host, &addr, 2, 0);
		if (peer == nullptr) {
			return false;
		}

		if (
			auto event = ENetEvent();
			enet_host_service(m_pImpl->host, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT
		) {
			puts("Connection successful.");
			return true;
		} else {
			puts("Connection failed.");
			enet_peer_reset(peer);
			return false;
		}
	}

	auto Client::IsConnected() const -> bool
	{
		return
			m_pImpl->host->peers[0].state == ENET_PEER_STATE_CONNECTED ||
			m_pImpl->host->peers[0].state == ENET_PEER_STATE_CONNECTING ||
			m_pImpl->host->peers[0].state == ENET_PEER_STATE_ACKNOWLEDGING_CONNECT ||
			m_pImpl->host->peers[0].state == ENET_PEER_STATE_CONNECTION_PENDING ||
			m_pImpl->host->peers[0].state == ENET_PEER_STATE_CONNECTION_SUCCEEDED;
	}

	auto Client::Disconnect() -> void
	{
		puts("Disconnecting...");

		enet_peer_disconnect(&(m_pImpl->host->peers[0]), 0);

		auto event = ENetEvent();
		while (enet_host_service(m_pImpl->host, &event, 3000) > 0) {
			switch (event.type) {
			case ENET_EVENT_TYPE_DISCONNECT:
				event.peer->data = nullptr;
				puts("Disconnected.");
				return;
			case ENET_EVENT_TYPE_RECEIVE:
				enet_packet_destroy(event.packet);
			}
		}

		puts("Disconnect failed. Resetting the peer.");
		enet_peer_reset(&(m_pImpl->host->peers[0]));
	}

	auto Client::Update() -> void
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

	auto Client::Send(Packet packet, U8 channel /*= 0*/) -> bool
	{
		return enet_peer_send(
			&(m_pImpl->host->peers[0]),
			channel, static_cast<ENetPacket*>(packet.Handle())
		) == 0;
	}

	auto Client::OnPacketReceived(PacketReceivedCallback callback) -> void
	{
		m_pImpl->cbPacketReceived = std::move(callback);
	}
}

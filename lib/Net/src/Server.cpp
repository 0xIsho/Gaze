#include "Net/Server.hpp"

#include "Debug/Assert.hpp"

#include <enet/enet.h>

namespace Gaze::Net {
	struct Server::Impl
	{
		ENetHost* host;
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
		auto event = ENetEvent();
		while (enet_host_service(m_pImpl->host, &event, 0) > 0) {
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
				enet_packet_destroy(event.packet);
				break;
			}
		}
	}
}

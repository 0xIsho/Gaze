#include "Net/Packet.hpp"

#include "Debug/Assert.hpp"

#include <enet/enet.h>

#include <utility>

namespace Gaze::Net {
	struct Packet::Impl
	{
		ENetPacket* packet = nullptr;
		bool hasOwnership = true;
	};

	Packet::Packet(const void* data, U64 size)
		: m_pImpl(new Impl())
	{
		m_pImpl->packet = enet_packet_create(data, size, ENET_PACKET_FLAG_NO_ALLOCATE);

		GAZE_ASSERT(m_pImpl->packet, "Failed to create a packet");
	}

	Packet::Packet(const Packet& other)
		: m_pImpl(new Impl())
	{
		if (this != &other) {
			if (m_pImpl->packet && m_pImpl->hasOwnership) {
				enet_packet_destroy(m_pImpl->packet);
			}

			m_pImpl->packet = other.m_pImpl->packet;
			other.m_pImpl->hasOwnership = false;
		}

		GAZE_ASSERT(m_pImpl->packet, "Failed to create a packet");
	}

	Packet::Packet(Packet&& other) noexcept
	{
		if (this != &other) {
			m_pImpl = std::exchange(other.m_pImpl, nullptr);
		}
	}

	Packet::~Packet()
	{
		if (m_pImpl) {
			if (m_pImpl->hasOwnership) {
				enet_packet_destroy(m_pImpl->packet);
			}

			delete m_pImpl;
		}
	}

	auto Packet::operator=(const Packet& other) -> Packet&
	{
		if (this != &other) {
			if (m_pImpl->packet && m_pImpl->hasOwnership) {
				enet_packet_destroy(m_pImpl->packet);
			}

			m_pImpl->packet = other.m_pImpl->packet;
			other.m_pImpl->hasOwnership = false;
		}

		return *this;
	}

	auto Packet::operator=(Packet&& other) noexcept -> Packet&
	{
		if (this != &other) {
			m_pImpl = std::exchange(other.m_pImpl, nullptr);
		}

		return *this;
	}

	auto Packet::Handle() const -> void*
	{
		return reinterpret_cast<void*>(m_pImpl->packet);
	}

	auto Packet::HasOwnership(bool val) -> void
	{
		m_pImpl->hasOwnership = val;
	}
}

#pragma once

#include "Core/Type.hpp"

#include "Net/Type.hpp"
#include "Net/Packet.hpp"

#include <string_view>

namespace Gaze::Net {
	class Client
	{
		struct Impl;

	public:
		Client();
		~Client();

		[[nodiscard]]
		auto Connect(std::string_view host, U16 port) -> bool;
		[[nodiscard]]
		auto IsConnected() const -> bool;
		auto Disconnect() -> void;

		auto Update() -> void;

		auto Send(Packet packet, U8 channel = 0) -> bool;

		auto OnPacketReceived(PacketReceivedCallback callback) -> void;

	private:
		Impl* m_pImpl;
	};
}

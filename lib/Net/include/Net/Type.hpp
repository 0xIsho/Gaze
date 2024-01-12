#pragma once

#include "Core/Type.hpp"

#include "Net/Packet.hpp"

#include <functional>

namespace Gaze::Net {
	using PacketReceivedCallback = std::function<void(U32 senderID, Packet packet)>;
}

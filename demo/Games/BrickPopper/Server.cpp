#include "Client/App.hpp"

#include "Common.hpp"

#include <glm/vec2.hpp>

#include <cstring>
#include <algorithm>

using namespace Gaze;

class BrickPopperServer : public Client::ServerApp
{
public:
	BrickPopperServer(int argc, char** argv);

private:
	auto OnInit() -> Status override;
	auto OnUpdate(F64 deltaTime) -> void override;
	auto OnFixedUpdate(F64 deltaTime) -> void override;
	auto OnShutdown() -> Status override;
	auto OnPacketReceived(U32 /*sender*/, Net::Packet /*packet*/) -> void override;
	auto OnClientConnected(U32 clientID) -> void override;
	auto OnClientDisconnected(U32 clientID) -> void override;

	auto HandleCollision() -> void;

private:
	glm::vec2 m_PlayerPos;
	glm::vec2 m_PlayerDir;
};

BrickPopperServer::BrickPopperServer(int argc, char** argv)
	: ServerApp(argc, argv)
	, m_PlayerPos({ kWindowWidth / 2, kPaddleBottomOffset })
	, m_PlayerDir({})
{
}

auto BrickPopperServer::OnInit() -> Status
{
	return Status::Success;
}

auto BrickPopperServer::OnUpdate(F64 deltaTime) -> void
{

}

auto BrickPopperServer::OnFixedUpdate(F64 deltaTime) -> void
{
	m_PlayerPos.x += m_PlayerDir.x * kPaddleSpeed * deltaTime;

	HandleCollision();

	auto serverPacket = ServerPacket{
		{ I32(m_PlayerPos.x), I32(m_PlayerPos.y) },
		{ Byte(m_PlayerDir.x), Byte(m_PlayerDir.y) }
	};

	Broadcast(Net::Packet(reinterpret_cast<void*>(&serverPacket), sizeof(ServerPacket)));
}

auto BrickPopperServer::HandleCollision() -> void
{
	m_PlayerPos.x = std::clamp(m_PlayerPos.x, kPaddleWidth / 2.F, kWindowWidth - kPaddleWidth / 2.F);
}

auto BrickPopperServer::OnShutdown() -> Status
{
	return Status::Success;
}

auto BrickPopperServer::OnPacketReceived(U32 /*sender*/, Net::Packet packet) -> void
{
	auto clientPacket = ClientPacket();
	std::memcpy(&clientPacket, packet.Data(), sizeof(ClientPacket));

	if (clientPacket.left == 1) {
		m_PlayerDir.x = -1;
	}
	else if (clientPacket.right == 1) {
		m_PlayerDir.x = 1;
	}
	else {
		m_PlayerDir.x = 0;
	}
}

auto BrickPopperServer::OnClientConnected(U32 clientID) -> void
{

}

auto BrickPopperServer::OnClientDisconnected(U32 clientID) -> void
{

}

GAZE_REGISTER_APP(BrickPopperServer);

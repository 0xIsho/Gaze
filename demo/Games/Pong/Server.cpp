#include "Client/App.hpp"

#include "Core/Type.hpp"

#include "Net/Packet.hpp"

#include "Type.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include <cstring>
#include <chrono>
#include <algorithm>

using namespace Gaze;

class PongServer : public Client::ServerApp
{
public:
	PongServer(int argc, char** argv);

private:
	auto OnInit() -> Status override;
	auto OnUpdate(F64 deltaTime) -> void override;
	auto OnFixedUpdate(F64 deltaTime) -> void override;
	auto OnShutdown() -> Status override;
	auto OnPacketReceived(U32 /*sender*/, Net::Packet /*packet*/) -> void override;
	auto OnClientConnected(U32 clientID) -> void override;
	auto OnClientDisconnected(U32 clientID) -> void override;

	auto HandleCollision() -> void;
	auto Reset() -> void;

private:
	U32 m_P1ID = U32(-1);
	U32 m_P2ID = U32(-1);
	static constexpr auto kInvalidID = U32(-1);

	glm::vec2 m_P1Pos;
	glm::vec2 m_P1Dir;
	I32 m_P1Score{};
	glm::vec2 m_P2Pos;
	glm::vec2 m_P2Dir;
	I32 m_P2Score{};

	glm::vec2 m_BallPos;
	glm::vec2 m_BallDir;

	bool m_ArePlayersReady = false; /**< True when both players are connected */
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> m_PlayersReadyTimestamp;
	bool m_HasGameStarted = false; /**< Starts after "m_StartDelay" after both players connect */
	std::chrono::duration<long long> m_StartDelay = std::chrono::seconds(3);
};

PongServer::PongServer(int argc, char** argv)
	: ServerApp(argc, argv)
{
	srand(U32(time(nullptr)));
	Reset();
}

auto PongServer::OnInit() -> Status
{
	return Status::Success;
}

auto PongServer::OnUpdate(F64 /*deltaTime*/) -> void
{
	if (m_ArePlayersReady && std::chrono::steady_clock::now() - m_PlayersReadyTimestamp >= m_StartDelay) {
		m_HasGameStarted = true;
	}
}

auto PongServer::OnFixedUpdate(F64 deltaTime) -> void
{
	if (!m_HasGameStarted) {
		return;
	}

	m_P1Pos.y += m_P1Dir.y * kPaddleSpeed * F32(deltaTime);
	m_P2Pos.y += m_P2Dir.y * kPaddleSpeed * F32(deltaTime);
	m_BallPos.x += m_BallDir.x * kBallSpeed * F32(deltaTime);
	m_BallPos.y += m_BallDir.y * kBallSpeed * F32(deltaTime);

	HandleCollision();

	auto packet = ServerPacket {
		{ I32(m_P1Pos.x)              , I32(m_P1Pos.y)               },
		{ I32(m_P1Dir.x * 1'000'000)  , I32(m_P1Dir.y * 1'000'000)   },
		{ I32(m_P2Pos.x)              , I32(m_P2Pos.y)               },
		{ I32(m_P2Dir.x * 1'000'000)  , I32(m_P2Dir.y * 1'000'000)   },
		{ I32(m_BallPos.x)            , I32(m_BallPos.y)             },
		{ I32(m_BallDir.x * 1'000'000), I32(m_BallDir.y * 1'000'000) }
	};
	Broadcast(Net::Packet(reinterpret_cast<const void*>(&packet), sizeof(ServerPacket)));
}

auto PongServer::OnShutdown() -> Status
{
	return Status::Success;
}

auto PongServer::OnPacketReceived(U32 sender, Net::Packet packet) -> void
{
	auto clientPacket = ClientPacket();
	std::memcpy(&clientPacket, packet.Data(), sizeof(ClientPacket));

	auto* dir =
		sender == m_P1ID ? &m_P1Dir
		: sender == m_P2ID ? &m_P2Dir
		: nullptr;

	if (dir == nullptr) {
		return;
	}

	if (clientPacket.start) {
		// TODO
	}
	if (clientPacket.up) {
		dir->y = -1;
	} else if (clientPacket.down) {
		dir->y = 1;
	} else {
		dir->y = 0;
	}
}

auto PongServer::OnClientConnected(U32 clientID) -> void
{
	if (m_P1ID == kInvalidID) {
		m_P1ID = clientID;
	} else if (m_P2ID == kInvalidID) {
		m_P2ID = clientID;
	}

	if (m_P1ID != kInvalidID && m_P2ID != kInvalidID) {
		m_ArePlayersReady = true;
		m_PlayersReadyTimestamp = std::chrono::steady_clock::now();
	}
}

auto PongServer::OnClientDisconnected(U32 clientID) -> void
{
	if (clientID == m_P1ID) {
		m_P1ID = kInvalidID;
	}
	if (clientID == m_P2ID) {
		m_P2ID = kInvalidID;
	}

	if (m_P1ID == kInvalidID || m_P2ID == kInvalidID) {
		Reset();
		m_ArePlayersReady = false;
	}
}

auto PongServer::HandleCollision() -> void
{
	m_P1Pos.y = std::clamp(m_P1Pos.y, 0.F, kWinHeight - kPaddleSize.y);
	m_P2Pos.y = std::clamp(m_P2Pos.y, 0.F, kWinHeight - kPaddleSize.y);

	if (
		const auto top = m_BallPos.y <= 0.F,
		bottom = m_BallPos.y + kBallSize >= kWinHeight;
		top || bottom
	) {
		if (top) {
			m_BallPos.y = 1;
		} else {
			m_BallPos.y = kWinHeight - kBallSize - 1;
		}

		m_BallDir.y *= -1;
	}
	if (
		const auto left = m_BallPos.x <= 0,
		right = m_BallPos.x + kBallSize >= kWinWidth;
		left || right
	) {
		if (left) {
			m_P2Score++;
		} else {
			m_P1Score++;
		}

		Reset();
		return;
	}

	if (m_BallPos.x + kBallSize >= m_P1Pos.x && m_BallPos.x <= m_P1Pos.x + kPaddleSize.x &&
		m_BallPos.y + kBallSize >= m_P1Pos.y && m_BallPos.y <= m_P1Pos.y + kPaddleSize.y) {
		m_BallDir = glm::normalize((m_BallPos + kBallSize / 2) - (m_P1Pos + kPaddleSize * .5F));
	}
	if (m_BallPos.x + kBallSize >= m_P2Pos.x && m_BallPos.x <= m_P2Pos.x + kPaddleSize.x &&
		m_BallPos.y + kBallSize >= m_P2Pos.y && m_BallPos.y <= m_P2Pos.y + kPaddleSize.y) {
		m_BallDir = glm::normalize((m_BallPos + kBallSize / 2) - (m_P2Pos + kPaddleSize * .5F));
	}
}

auto PongServer::Reset() -> void
{
	m_P1Pos   = { kDefaultPaddleOffsetX, kWinHeight / 2 - kPaddleSize.y / 2 };
	m_P1Dir   = { 0, 0 };
	m_P2Pos   = { kWinWidth - kPaddleSize.x - kDefaultPaddleOffsetX, kWinHeight / 2 - kPaddleSize.y / 2 };
	m_P2Dir   = { 0, 0 };
	m_BallPos = { kWinWidth / 2 - kBallSize / 2, kWinHeight / 2 - kBallSize / 2 };

	m_BallDir = glm::normalize(glm::vec2{ rand() - RAND_MAX, 0 });
	m_HasGameStarted = false;
}

GAZE_REGISTER_APP(PongServer);

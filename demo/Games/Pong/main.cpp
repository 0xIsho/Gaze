#include "Client/App.hpp"

#include "Core/Type.hpp"

#include "WM/Window.hpp"

#include "GFX/Mesh.hpp"
#include "GFX/Renderer.hpp"

#include "Input/Input.hpp"
#include "Input/KeyCode.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <ctime>
#include <random>
#include <cstdlib>

using namespace Gaze;

class MyApp : public Client::App
{
public:
	MyApp(int argc, char** argv);

private:
	auto OnInit() -> Status;
	auto OnUpdate(F64 deltaTime) -> void;
	auto OnFixedUpdate(F64 deltaTime) -> void;
	auto OnShutdown() -> Status;

	auto RenderPlayground() -> void;
	auto RenderPlayers() -> void;
	auto RenderScoreboard() -> void;
	auto HandleInput(F64 deltaTime) -> void;
	auto HandleCollision() -> void;
	auto Reset() -> void;

private:
	Mem::Shared<WM::Window> m_Win;
	Mem::Unique<GFX::Renderer> m_Rdr;
	Input::Handler m_Input;

	glm::vec3 m_P1Pos;
	I32 m_P1Score{};
	glm::vec3 m_P2Pos;
	I32 m_P2Score{};

	glm::vec3 m_BallPos;
	glm::vec2 m_BallDir;

	bool m_GameStarted{ false };

	static constexpr auto kWinWidth = 800;
	static constexpr auto kWinHeight = 400;
	static constexpr auto kWallThickness = 30.F;
	static constexpr auto kPaddleSize = glm::vec2(10.F, 50.F);
	static constexpr auto kPaddleSpeed = 200.0F;
	static constexpr auto kBallSize = 10.F;
	static constexpr auto kBallSpeed = 400.F;
};

MyApp::MyApp(int argc, char** argv)
	: App(argc, argv)
	, m_Win(Mem::MakeShared<WM::Window>("Gaze - Pong", kWinWidth, kWinHeight))
	, m_Rdr(GFX::CreateRenderer(m_Win))
	, m_Input(m_Win)
{
	srand(U32(time(nullptr)));
	Reset();
}

auto MyApp::OnInit() -> Status
{
	m_Win->OnClose([this] {
		Quit();
	});
	m_Win->Show();

	m_Rdr->SetProjection(glm::ortho(.0F, F32(kWinWidth), F32(kWinHeight), .0F, .1F, 10.F));

	return Status::Success;
}

auto MyApp::OnUpdate(F64 /*deltaTime*/) -> void
{
	if (m_Input.IsKeyPressed(Input::Key::kEnter) || m_Input.IsKeyPressed(Input::Key::kSpace)) {
		m_GameStarted = true;
	}

	m_Rdr->Clear();

	RenderPlayground();
	RenderPlayers();
	RenderScoreboard();

	m_Rdr->Render();
}

auto MyApp::OnFixedUpdate(F64 deltaTime) -> void
{
	if (!m_GameStarted) {
		return;
	}

	m_BallPos.x += m_BallDir.x * kBallSpeed * F32(deltaTime);
	m_BallPos.y += m_BallDir.y * kBallSpeed * F32(deltaTime);

	HandleInput(deltaTime);
	HandleCollision();
}

auto MyApp::OnShutdown() -> Status
{
	return Status::Success;
}

auto MyApp::RenderPlayground() -> void
{
	const auto walls = GFX::Mesh({
		{ .0F      , .0F           , .0F },
		{ .0F      , kWallThickness, .0F },
		{ kWinWidth, .0F           , .0F },
		{ kWinWidth, kWallThickness, .0F },

		{ .0F      , kWinHeight - kWallThickness, .0F },
		{ .0F      , kWinHeight                 , .0F },
		{ kWinWidth, kWinHeight - kWallThickness, .0F },
		{ kWinWidth, kWinHeight                 , .0F },
	},
	{
		0, 1, 2, 2, 1, 3,
		4, 5, 6, 6, 5, 7,
	});

	const auto midLine = GFX::Mesh({
		{ kWinWidth / 2, kWallThickness             , .0F },
		{ kWinWidth / 2, kWinHeight - kWallThickness, .0F }
	},
	{
		0, 1
	});

	m_Rdr->DrawMesh(walls, GFX::Renderer::PrimitiveMode::Triangles);
	m_Rdr->DrawMesh(midLine, GFX::Renderer::PrimitiveMode::Lines);
}

auto MyApp::RenderPlayers() -> void
{
	auto paddle = GFX::Mesh({
		{ .0F               , .0F          , .0F },
		{ .0F               , kPaddleSize.y, .0F },
		{ kPaddleSize.x      , .0F          , .0F },
		{ kPaddleSize.x      , kPaddleSize.y, .0F }
	},
	{
		0, 1, 2, 2, 1, 3
	});

	auto ball = GFX::Mesh({
		{ .0F      , .0F      , .0F },
		{ .0F      , kBallSize, .0F },
		{ kBallSize, .0F      , .0F },
		{ kBallSize, kBallSize, .0F }
	},
	{
		0, 1, 2, 2, 1, 3
	});

	paddle.SetPosition(m_P1Pos);
	m_Rdr->DrawMesh(paddle, GFX::Renderer::PrimitiveMode::Triangles);

	paddle.SetPosition(m_P2Pos);
	m_Rdr->DrawMesh(paddle, GFX::Renderer::PrimitiveMode::Triangles);

	ball.SetPosition(m_BallPos);
	m_Rdr->DrawMesh(ball, GFX::Renderer::PrimitiveMode::Triangles);
}

auto MyApp::RenderScoreboard() -> void
{
	auto p1ScoreboardPos = glm::vec3{
		kWinWidth / 4 - (m_P1Score + 10),
		kWallThickness + 10,
		.0F
	};
	auto p2ScoreboardPos = glm::vec3{
		kWinWidth - kWinWidth / 4 - (m_P2Score + 10),
		kWallThickness + 10,
		.0F
	};

	auto line = GFX::Mesh({
		{ .0F, .0F, .0F },
		{ .0F, 10.F, .0F }
	},
	{
		0, 1
	});

	for (auto i = I32(0); i < m_P1Score; i++) {
		line.SetPosition(p1ScoreboardPos);
		m_Rdr->DrawMesh(line, GFX::Renderer::PrimitiveMode::Lines);
		p1ScoreboardPos.x += 5;
	}
	for (auto i = I32(0); i < m_P2Score; i++) {
		line.SetPosition(p2ScoreboardPos);
		m_Rdr->DrawMesh(line, GFX::Renderer::PrimitiveMode::Lines);
		p2ScoreboardPos.x += 5;
	}
}

auto MyApp::HandleInput(F64 deltaTime) -> void
{
	if (m_Input.IsKeyPressed(Input::Key::kW)) {
		m_P1Pos.y -= kPaddleSpeed * F32(deltaTime);
	}
	if (m_Input.IsKeyPressed(Input::Key::kS)) {
		m_P1Pos.y += kPaddleSpeed * F32(deltaTime);
	}
	if (m_Input.IsKeyPressed(Input::Key::kUp)) {
		m_P2Pos.y -= kPaddleSpeed * F32(deltaTime);
	}
	if (m_Input.IsKeyPressed(Input::Key::kDown)) {
		m_P2Pos.y += kPaddleSpeed * F32(deltaTime);
	}
}

auto MyApp::HandleCollision() -> void
{
	m_P1Pos.y = std::clamp(m_P1Pos.y, kWallThickness, kWinHeight - kWallThickness - kPaddleSize.y);
	m_P2Pos.y = std::clamp(m_P2Pos.y, kWallThickness, kWinHeight - kWallThickness - kPaddleSize.y);

	if (
		const auto top = m_BallPos.y <= kWallThickness,
		bottom = m_BallPos.y + kBallSize >= kWinHeight - kWallThickness;
		top || bottom
	) {
		if (top) {
			m_BallPos.y = kWallThickness + 1;
		} else {
			m_BallPos.y = kWinHeight - kBallSize - kWallThickness - 1;
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
		m_BallDir = glm::normalize((m_BallPos + kBallSize / 2) - (m_P1Pos + glm::vec3{ kPaddleSize * .5F, .0F }));
	}
	if (m_BallPos.x + kBallSize >= m_P2Pos.x && m_BallPos.x <= m_P2Pos.x + kPaddleSize.x &&
		m_BallPos.y + kBallSize >= m_P2Pos.y && m_BallPos.y <= m_P2Pos.y + kPaddleSize.y) {
		m_BallDir = glm::normalize((m_BallPos + kBallSize / 2) - (m_P2Pos + glm::vec3{ kPaddleSize * .5F, .0F }));
	}
}

auto MyApp::Reset() -> void
{
	m_P1Pos = { kWallThickness, kWinHeight / 2 - kPaddleSize.y / 2, .0F };
	m_P2Pos = { kWinWidth - kWallThickness - kPaddleSize.x, kWinHeight / 2 - kPaddleSize.y / 2, .0F };
	m_BallPos = { kWinWidth / 2 - kBallSize / 2, kWinHeight / 2 - kBallSize / 2, .0F };

	m_BallDir = { rand() - RAND_MAX / 2, rand() - RAND_MAX / 2 };
	m_BallDir = glm::normalize(m_BallDir);

	m_GameStarted = false;
}

GAZE_REGISTER_APP(MyApp);

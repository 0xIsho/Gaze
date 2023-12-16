#include "Client/App.hpp"

#include "Core/Type.hpp"

#include "WM/Window.hpp"

#include "GFX/Mesh.hpp"
#include "GFX/Renderer.hpp"
#include "GFX/Primitives.hpp"

#include "Input/KeyCode.hpp"

#include "Events/Dispatcher.hpp"
#include "Events/WindowEvent.hpp"
#include "Events/KeyEvent.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <ctime>
#include <random>
#include <cstdlib>

using namespace Gaze;

class MyApp : public Client::ClientApp
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
	auto HandleCollision() -> void;
	auto Reset() -> void;

private:
	Mem::Shared<WM::Window> m_Win;
	Mem::Unique<GFX::Renderer> m_Rdr;

	glm::vec3 m_P1Pos;
	glm::vec3 m_P1Dir;
	I32 m_P1Score{};
	glm::vec3 m_P2Pos;
	glm::vec3 m_P2Dir;
	I32 m_P2Score{};

	glm::vec3 m_BallPos;
	glm::vec2 m_BallDir;

	bool m_GameStarted{ false };

	GFX::Mesh m_PaddleMesh;
	GFX::Mesh m_BallMesh;
	GFX::Mesh m_ScoreMarkerMesh;
	GFX::Mesh m_MidlineMesh;

	static constexpr auto kWinWidth = 800;
	static constexpr auto kWinHeight = 400;
	static constexpr auto kPaddleSize = glm::vec2(10.F, 50.F);
	static constexpr auto kPaddleSpeed = 200.0F;
	static constexpr auto kBallSize = 10.F;
	static constexpr auto kBallSpeed = 400.F;
};

MyApp::MyApp(int argc, char** argv)
	: ClientApp(argc, argv)
	, m_Win(Mem::MakeShared<WM::Window>("Gaze - Pong", kWinWidth, kWinHeight))
	, m_Rdr(GFX::CreateRenderer(m_Win))
	, m_PaddleMesh(GFX::CreateQuad({ }, kPaddleSize.x, kPaddleSize.y))
	, m_BallMesh(GFX::CreateQuad({ }, kBallSize, kBallSize))
	, m_ScoreMarkerMesh(GFX::CreateLine({ .0F, .0F, .0F }, { .0F, 10.F, .0F }))
	, m_MidlineMesh(GFX::CreateLine({ kWinWidth / 2, .0F, .0F }, { kWinWidth / 2, kWinHeight, .0F }))
{
	m_PaddleMesh.Rotate(glm::radians(-90.0F), { 1.0F, .0F, .0F });
	m_BallMesh.Rotate(glm::radians(-90.0F), { 1.0F, .0F, .0F });

	srand(U32(time(nullptr)));
	Reset();
}

auto MyApp::OnInit() -> Status
{
	m_Win->OnEvent([this](auto& event) {
		auto dispatcher = Events::Dispatcher(event);

		dispatcher.Dispatch<Events::WindowClose>([this](auto&) {
			Quit();
		});

		dispatcher.Dispatch<Events::KeyPressed>([this](auto& event) {
			if (event.Keycode() == Input::Key::kW) {
				m_P1Dir.y -= 1;
			}
			if (event.Keycode() == Input::Key::kS) {
				m_P1Dir.y += 1;
			}
			if (event.Keycode() == Input::Key::kUp) {
				m_P2Dir.y -= 1;
			}
			if (event.Keycode() == Input::Key::kDown) {
				m_P2Dir.y += 1;
			}
		});

		dispatcher.Dispatch<Events::KeyReleased>([this](auto& event) {
			if (event.Keycode() == Input::Key::kEnter || event.Keycode() == Input::Key::kSpace) {
				m_GameStarted = true;
			}

			if (event.Keycode() == Input::Key::kW) {
				m_P1Dir.y += 1;
			}
			if (event.Keycode() == Input::Key::kS) {
				m_P1Dir.y -= 1;
			}
			if (event.Keycode() == Input::Key::kUp) {
				m_P2Dir.y += 1;
			}
			if (event.Keycode() == Input::Key::kDown) {
				m_P2Dir.y -= 1;
			}
		});
	});

	m_Win->Show();

	m_Rdr->SetProjection(glm::ortho(.0F, F32(kWinWidth), F32(kWinHeight), .0F, .1F, 10.F));

	return Status::Success;
}

auto MyApp::OnUpdate(F64 /*deltaTime*/) -> void
{
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

	m_P1Pos += m_P1Dir * kPaddleSpeed * F32(deltaTime);
	m_P2Pos += m_P2Dir * kPaddleSpeed * F32(deltaTime);

	m_BallPos.x += m_BallDir.x * kBallSpeed * F32(deltaTime);
	m_BallPos.y += m_BallDir.y * kBallSpeed * F32(deltaTime);

	HandleCollision();
}

auto MyApp::OnShutdown() -> Status
{
	return Status::Success;
}

auto MyApp::RenderPlayground() -> void
{
	m_Rdr->DrawMesh(m_MidlineMesh, GFX::Renderer::PrimitiveMode::Lines);
}

auto MyApp::RenderPlayers() -> void
{
	m_PaddleMesh.SetPosition(m_P1Pos);
	m_Rdr->DrawMesh(m_PaddleMesh, GFX::Renderer::PrimitiveMode::Triangles);

	m_PaddleMesh.SetPosition(m_P2Pos);
	m_Rdr->DrawMesh(m_PaddleMesh, GFX::Renderer::PrimitiveMode::Triangles);

	m_BallMesh.SetPosition(m_BallPos);
	m_Rdr->DrawMesh(m_BallMesh, GFX::Renderer::PrimitiveMode::Triangles);
}

auto MyApp::RenderScoreboard() -> void
{
	auto p1ScoreboardPos = glm::vec3{ kWinWidth / 4 - (m_P1Score + 10)            , 10, .0F };
	auto p2ScoreboardPos = glm::vec3{ kWinWidth - kWinWidth / 4 - (m_P2Score + 10), 10, .0F };

	for (auto i = 0; i < m_P1Score; i++) {
		m_ScoreMarkerMesh.SetPosition(p1ScoreboardPos);
		m_Rdr->DrawMesh(m_ScoreMarkerMesh, GFX::Renderer::PrimitiveMode::Lines);
		p1ScoreboardPos.x += 5;
	}
	for (auto i = 0; i < m_P2Score; i++) {
		m_ScoreMarkerMesh.SetPosition(p2ScoreboardPos);
		m_Rdr->DrawMesh(m_ScoreMarkerMesh, GFX::Renderer::PrimitiveMode::Lines);
		p2ScoreboardPos.x += 5;
	}
}

auto MyApp::HandleCollision() -> void
{
	m_P1Pos.y = std::clamp(m_P1Pos.y, .0F, kWinHeight - kPaddleSize.y / 2);
	m_P2Pos.y = std::clamp(m_P2Pos.y, .0F, kWinHeight - kPaddleSize.y / 2);

	if (
		const auto top = m_BallPos.y <= .0F,
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

	const auto p1PaddleBox = glm::vec3{ m_P1Pos.x - kPaddleSize.x / 2, m_P1Pos.y - kPaddleSize.y / 2, .0F };
	const auto p2PaddleBox = glm::vec3{ m_P2Pos.x - kPaddleSize.x / 2, m_P2Pos.y - kPaddleSize.y / 2, .0F };
	const auto ballBox     = glm::vec3{ m_BallPos.x - kBallSize / 2  , m_BallPos.y - kBallSize / 2  , .0F };

	if (ballBox.x + kBallSize >= p1PaddleBox.x && ballBox.x <= p1PaddleBox.x + kPaddleSize.x &&
		ballBox.y + kBallSize >= p1PaddleBox.y && ballBox.y <= p1PaddleBox.y + kPaddleSize.y) {
		m_BallDir = glm::normalize((m_BallPos + kBallSize / 2) - (p1PaddleBox + glm::vec3{ kPaddleSize.x / 2 - 10, kPaddleSize.y / 2, .0F }));
	}
	if (ballBox.x + kBallSize >= p2PaddleBox.x && ballBox.x <= p2PaddleBox.x + kPaddleSize.x &&
		ballBox.y + kBallSize >= p2PaddleBox.y && ballBox.y <= p2PaddleBox.y + kPaddleSize.y) {
		m_BallDir = glm::normalize((m_BallPos + kBallSize / 2) - (p2PaddleBox + glm::vec3{ kPaddleSize.x / 2 + 10, kPaddleSize.y / 2, .0F }));
	}
}

auto MyApp::Reset() -> void
{
	m_P1Pos   = { 30.F               , F32(kWinHeight) / 2, .0F };
	m_P2Pos   = { kWinWidth - 30.0F  , F32(kWinHeight) / 2, .0F };
	m_BallPos = { F32(kWinWidth) / 2, F32(kWinHeight) / 2, .0F };

	m_P1Dir = {};
	m_P2Dir = {};

	m_BallDir = { rand() - RAND_MAX / 2, rand() - RAND_MAX / 2 };
	m_BallDir = glm::normalize(m_BallDir);

	m_GameStarted = false;
}

GAZE_REGISTER_APP(MyApp);

#include "Client/App.hpp"

#include "Core/Type.hpp"

#include "WM/Window.hpp"

#include "GFX/Mesh.hpp"
#include "GFX/Renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace Gaze;

class MyApp : public Client::App
{
public:
	MyApp(int argc, char** argv);

private:
	auto OnInit() -> Status;
	auto OnUpdate(F64 deltaTime) -> void;
	auto OnShutdown() -> Status;

	auto RenderPlayground() -> void;
	auto RenderPlayers() -> void;

private:
	Mem::Shared<WM::Window> m_Win;
	Mem::Unique<GFX::Renderer> m_Rdr;

	glm::vec3 m_P1Pos;
	glm::vec3 m_P2Pos;

	static constexpr auto kWinWidth = 800;
	static constexpr auto kWinHeight = 400;
	static constexpr auto kWallThickness = 30.F;
	static constexpr auto kPaddleWidth = 10.F;
	static constexpr auto kPaddleHeight = 50.F;
};

MyApp::MyApp(int argc, char** argv)
	: App(argc, argv)
	, m_Win(Mem::MakeShared<WM::Window>("Gaze - Pong", kWinWidth, kWinHeight))
	, m_Rdr(GFX::CreateRenderer(m_Win))
	, m_P1Pos({ kWallThickness, kWinHeight / 2 - kPaddleHeight / 2, .0F })
	, m_P2Pos({ kWinWidth - kWallThickness - kPaddleWidth, kWinHeight / 2 - kPaddleHeight / 2, .0F })
{
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

auto MyApp::OnUpdate(F64 deltaTime) -> void
{
	m_Rdr->Clear();

	RenderPlayground();
	RenderPlayers();

	m_Rdr->Render();
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
		{ .0F               , kPaddleHeight, .0F },
		{ kPaddleWidth      , .0F          , .0F },
		{ kPaddleWidth      , kPaddleHeight, .0F }
	},
	{
		0, 1, 2, 2, 1, 3
	});

	paddle.SetPosition(m_P1Pos);
	m_Rdr->DrawMesh(paddle, GFX::Renderer::PrimitiveMode::Triangles);

	paddle.SetPosition(m_P2Pos);
	m_Rdr->DrawMesh(paddle, GFX::Renderer::PrimitiveMode::Triangles);
}

GAZE_REGISTER_APP(MyApp);

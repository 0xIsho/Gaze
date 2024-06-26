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

#include "Type.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <cstring>

using namespace Gaze;

class MyApp : public Client::ClientApp
{
public:
	MyApp(int argc, char** argv);

private:
	auto OnInit()                                         -> Status override;
	auto OnUpdate(F64 deltaTime)                          -> void override;
	auto OnFixedUpdate(F64 deltaTime)                     -> void override;
	auto OnShutdown()                                     -> Status override;
	auto OnPacketReceived(U32 sender, Net::Packet packet) -> void override;

	auto RenderPlayground() -> void;
	auto RenderPlayers()    -> void;

private:
	Shared<WM::Window>    m_Win;
	Unique<GFX::Renderer> m_Rdr;

	glm::vec2                  m_P1Pos;
	glm::vec2                  m_P1Dir;
	I32                        m_P1Score{};

	glm::vec2                  m_P2Pos;
	glm::vec2                  m_P2Dir;
	I32                        m_P2Score{};

	glm::vec2                  m_BallPos;
	glm::vec2                  m_BallDir;

	GFX::Object                m_Paddle;
	GFX::Object                m_Ball;
	GFX::Object                m_Midline;

	ClientPacket               m_Packet = {};
	bool                       m_IsPacketDirty = true;
};

MyApp::MyApp(int argc, char** argv)
	: ClientApp(argc, argv)
	, m_Win(MakeShared<WM::Window>("Gaze - Pong", kWinWidth, kWinHeight))
	, m_Rdr(GFX::CreateRenderer(m_Win))
	, m_P1Pos(kDefaultPaddleOffsetX, kWinHeight / 2 - kPaddleSize.y / 2)
	, m_P2Pos(kWinWidth - kPaddleSize.x - kDefaultPaddleOffsetX, kWinHeight / 2 - kPaddleSize.y / 2)
	, m_BallPos(kWinWidth / 2 - kBallSize / 2, kWinHeight / 2 - kBallSize / 2)
	, m_Paddle(GFX::Primitives::CreateQuad({}, kPaddleSize.x, kPaddleSize.y))
	, m_Ball(GFX::Primitives::CreateQuad({}, kBallSize, kBallSize))
	, m_Midline(GFX::Primitives::CreateLine({ kWinWidth / 2, .0F, .0F }, { kWinWidth / 2, kWinHeight, .0F }))
{
	m_Paddle.Rotate(glm::radians(-90.0F), glm::vec3{ 1.0F, .0F, .0F });
	m_Ball.Rotate(glm::radians(-90.0F), glm::vec3{ 1.0F, .0F, .0F });
}

auto MyApp::OnInit() -> Status
{
	m_Win->OnEvent([this](auto& event) {
		auto dispatcher = Events::Dispatcher(event);

		dispatcher.Dispatch<Events::WindowClose>([this](auto&) {
			Quit();
		});

		dispatcher.Dispatch<Events::KeyPressed>([this](auto& event) {
			switch (event.Keycode()) {
			case Input::Key::kW:
			case Input::Key::kUp:
				m_Packet.up = 1;
				break;
			case Input::Key::kS:
			case Input::Key::kDown:
				m_Packet.down = 1;
				break;
			case Input::Key::kEnter:
			case Input::Key::kSpace:
				m_Packet.start = 1;
				break;
			default:
				break;
			}

			m_IsPacketDirty = true;
		});

		dispatcher.Dispatch<Events::KeyReleased>([this](auto& event) {
			if (
				bool start = event.Keycode() == Input::Key::kEnter || event.Keycode() == Input::Key::kSpace,
				     up = event.Keycode() == Input::Key::kW || event.Keycode() == Input::Key::kUp,
				     down = event.Keycode() == Input::Key::kS || event.Keycode() == Input::Key::kDown;
				     start || up || down
			) {
				switch (event.Keycode()) {
				case Input::Key::kW:
				case Input::Key::kUp:
					m_Packet.up = 0;
					break;
				case Input::Key::kS:
				case Input::Key::kDown:
					m_Packet.down = 0;
					break;
				case Input::Key::kEnter:
				case Input::Key::kSpace:
					m_Packet.start = 0;
					break;
				default:
					break;
				}

				m_IsPacketDirty = true;
			}
		});
	});

	m_Win->Show();

	m_Rdr->SetCamera(MakeShared<GFX::OrthographicCamera>(.0F, F32(kWinWidth), F32(kWinHeight), .0F, .1F, 10.F));

	return Status::Success;
}

auto MyApp::OnUpdate(F64 /*deltaTime*/) -> void
{
	m_Rdr->Clear();

	RenderPlayground();
	RenderPlayers();

	if (m_IsPacketDirty) {
		Send(Net::Packet(reinterpret_cast<const void*>(&m_Packet), sizeof(ClientPacket)));
		m_IsPacketDirty = false;
	}

	m_Rdr->Render();
}

auto MyApp::OnFixedUpdate(F64 /*deltaTime*/) -> void
{
}

auto MyApp::OnShutdown() -> Status
{
	return Status::Success;
}

auto MyApp::RenderPlayground() -> void
{
	m_Rdr->SubmitObject(m_Midline, GFX::Renderer::PrimitiveMode::Lines);
}

auto MyApp::RenderPlayers() -> void
{
	m_Paddle.SetPosition({ m_P1Pos + kPaddleSize * .5F, 0.0F });
	m_Rdr->SubmitObject(m_Paddle, GFX::Renderer::PrimitiveMode::Triangles);

	m_Paddle.SetPosition({ m_P2Pos + kPaddleSize * .5F, 0.0F });
	m_Rdr->SubmitObject(m_Paddle, GFX::Renderer::PrimitiveMode::Triangles);

	m_Ball.SetPosition({ m_BallPos + kBallSize * .5F, 0.0F });
	m_Rdr->SubmitObject(m_Ball, GFX::Renderer::PrimitiveMode::Triangles);
}

auto MyApp::OnPacketReceived(U32 /*sender*/, Net::Packet packet) -> void
{
	auto gamePacket = ServerPacket();
	std::memcpy(&gamePacket, packet.Data(), sizeof(ServerPacket));

	m_P1Pos   = { gamePacket.p1Pos[0]                     , gamePacket.p1Pos[1]                      };
	m_P1Dir   = { F32(gamePacket.p1Dir[0])  / 1'000'000.F , F32(gamePacket.p1Dir[1]) / 1'000'000.F   };
	m_P2Pos   = { gamePacket.p2Pos[0]                     , gamePacket.p2Pos[1]                      };
	m_P2Dir   = { F32(gamePacket.p2Dir[0])  / 1'000'000.F , F32(gamePacket.p2Dir[1]) / 1'000'000.F   };
	m_BallPos = { gamePacket.ballPos[0]                   , gamePacket.ballPos[1]                    };
	m_BallDir = { F32(gamePacket.ballDir[0]) / 1'000'000.F, F32(gamePacket.ballDir[1]) / 1'000'000.F };
}

GAZE_REGISTER_APP(MyApp);

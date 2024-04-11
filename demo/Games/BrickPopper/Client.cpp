#include "Client/App.hpp"

#include "Common.hpp"

#include "WM/Window.hpp"

#include "Input/KeyCode.hpp"

#include "Events/Dispatcher.hpp"
#include "Events/Event.hpp"
#include "Events/WindowEvent.hpp"
#include "Events/KeyEvent.hpp"

#include "GFX/Renderer.hpp"
#include "GFX/Object.hpp"
#include "GFX/Primitives.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <cstring>

using namespace Gaze;

class BrickPopperClient : public Client::ClientApp
{
public:
	BrickPopperClient(int argc, char** argv);

private:
	auto OnInit() -> Status override;
	auto OnUpdate(F64 deltaTime) -> void override;
	auto OnFixedUpdate(F64 deltaTime) -> void override;
	auto OnShutdown() -> Status override;
	auto OnPacketReceived(U32 sender, Net::Packet packet) -> void override;

	auto RenderPlayers() -> void;

private:
	Shared<WM::Window> m_Window;
	Unique<GFX::Renderer> m_Renderer;

	GFX::Object m_Paddle;
	glm::vec2 m_PlayerPos;
	glm::vec2 m_PlayerDir;

	ClientPacket m_Packet;
	bool m_bIsPacketDirty = false;
};

BrickPopperClient::BrickPopperClient(int argc, char** argv)
	: ClientApp(argc, argv)
	, m_Paddle(GFX::Primitives::CreateQuad({}, kPaddleWidth, kPaddleHeight))
	, m_PlayerPos({ kWindowWidth / 2, kPaddleBottomOffset })
	, m_Packet({})
{
	m_Paddle.Rotate(glm::radians(-90.0F), glm::vec3{ 1.0F, .0F, .0F });
}

auto BrickPopperClient::OnInit() -> Status
{
	m_Window = MakeShared<WM::Window>("Brick Popper", kWindowWidth, kWindowHeight);
	m_Renderer = GFX::CreateRenderer(m_Window);

	if (m_Renderer == nullptr) {
		Logger().Error("Error creating a renderer.");
		return Status::Fail;
	}

	m_Renderer->SetCamera(MakeShared<GFX::OrthographicCamera>(0.F, F32(kWindowWidth), 0.F, F32(kWindowHeight), .1F, 10.F));

	m_Window->OnEvent([this](auto& event) {
		auto dispatcher = Events::Dispatcher(event);

		dispatcher.Dispatch<Events::WindowClose>([this](auto& event) {
			Quit();
		});

		dispatcher.Dispatch<Events::KeyPressed>([this](auto& event) {
			switch (event.Keycode()) {
			case Input::Key::kA:
			case Input::Key::kLeft:
				m_Packet.left = 1;
				m_bIsPacketDirty = true;
				break;
			case Input::Key::kD:
			case Input::Key::kRight:
				m_Packet.right = 1;
				m_bIsPacketDirty = true;
				break;
			default:
				break;
			}
		});
		dispatcher.Dispatch<Events::KeyReleased>([this](auto& event) {
			switch (event.Keycode()) {
			case Input::Key::kA:
			case Input::Key::kLeft:
				m_Packet.left = 0;
				m_bIsPacketDirty = true;
				break;
			case Input::Key::kD:
			case Input::Key::kRight:
				m_Packet.right = 0;
				m_bIsPacketDirty = true;
				break;
			default:
				break;
			}
			});
	});

	m_Window->Show();

	return Status::Success;
}

auto BrickPopperClient::OnUpdate(F64 deltaTime) -> void
{
	m_Renderer->Clear();

	RenderPlayers();

	m_Renderer->Render();

	if (m_bIsPacketDirty) {
		Send(Net::Packet(reinterpret_cast<void*>(&m_Packet), sizeof(ClientPacket)));
		m_bIsPacketDirty = false;
	}
}

auto BrickPopperClient::RenderPlayers() -> void
{
	m_Paddle.SetPosition({ m_PlayerPos, 0 });
	m_Renderer->SubmitObject(m_Paddle, GFX::Renderer::PrimitiveMode::Triangles);
}

auto BrickPopperClient::OnFixedUpdate(F64 deltaTime) -> void
{

}

auto BrickPopperClient::OnShutdown() -> Status
{
	return Status::Success;
}

auto BrickPopperClient::OnPacketReceived(U32 sender, Net::Packet packet) -> void
{
	auto serverPacket = ServerPacket();
	std::memcpy(&serverPacket, packet.Data(), sizeof(ServerPacket));

	m_PlayerPos = { serverPacket.playerPos[0], serverPacket.playerPos[1] };
	m_PlayerDir = { serverPacket.playerDir[0], serverPacket.playerDir[1] };
}

GAZE_REGISTER_APP(BrickPopperClient);

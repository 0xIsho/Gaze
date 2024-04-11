#include "Client/App.hpp"

#include "WM/Window.hpp"

#include "Events/Dispatcher.hpp"
#include "Events/Event.hpp"
#include "Events/WindowEvent.hpp"

#include "GFX/Renderer.hpp"
#include "GFX/Object.hpp"
#include "GFX/Primitives.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using namespace Gaze;

static constexpr auto kWindowWidth        = 1200;
static constexpr auto kWindowHeight       = 1024;
static constexpr auto kPaddleWidth        = 100;
static constexpr auto kPaddleHeight       = 10;
static constexpr auto kPaddleBottomOffset = 50;

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
};

BrickPopperClient::BrickPopperClient(int argc, char** argv)
	: ClientApp(argc, argv)
	, m_Paddle(GFX::Primitives::CreateQuad({}, kPaddleWidth, kPaddleHeight))
	, m_PlayerPos({ kWindowWidth / 2, kPaddleBottomOffset })
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
	});

	m_Window->Show();

	return Status::Success;
}

auto BrickPopperClient::OnUpdate(F64 deltaTime) -> void
{
	m_Renderer->Clear();

	RenderPlayers();

	m_Renderer->Render();
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

}

GAZE_REGISTER_APP(BrickPopperClient);

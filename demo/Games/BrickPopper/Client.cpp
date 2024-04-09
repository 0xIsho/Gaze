#include "Client/App.hpp"

#include "WM/Window.hpp"

#include "Events/Dispatcher.hpp"
#include "Events/Event.hpp"
#include "Events/WindowEvent.hpp"

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

private:
	WM::Window m_Window;
};

BrickPopperClient::BrickPopperClient(int argc, char** argv)
	: ClientApp(argc, argv)
	, m_Window("Brick Popper", 1200, 1024)
{
}

auto BrickPopperClient::OnInit() -> Status
{
	m_Window.OnEvent([this](auto& event) {
		auto dispatcher = Events::Dispatcher(event);

		dispatcher.Dispatch<Events::WindowClose>([this](auto& event) {
			Quit();
		});
	});

	m_Window.Show();

	return Status::Success;
}

auto BrickPopperClient::OnUpdate(F64 deltaTime) -> void
{

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

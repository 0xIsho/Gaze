#include "Client/App.hpp"

#include "Core/Type.hpp"
#include "WM/Window.hpp"

#include "Events/Dispatcher.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/WindowEvent.hpp"

using namespace Gaze;

class MyApp : public Client::ClientApp
{
public:
	MyApp(int argc, char** argv);

private:
	auto OnInit() -> Status override;
	auto OnUpdate(F64 deltaTime) -> void override;
	auto OnShutdown() -> Status override;

private:
	Shared<WM::Window> m_Win;
};

MyApp::MyApp(int argc, char** argv)
	: ClientApp(argc, argv)
	, m_Win(MakeShared<WM::Window>("Hello Input", 800, 600))
{
}

auto MyApp::OnInit() -> Status
{
	m_Win->OnEvent([this](auto& event) {
		auto dispatcher = Events::Dispatcher(event);

		dispatcher.Dispatch<Events::WindowClose>([this](auto&) {
			Quit();
		});

		dispatcher.Dispatch<Events::KeyReleased>([this](auto& event) {
			if (event.Keycode() == Input::Key::kEscape) {
				Quit();
			}
		});
	});

	m_Win->Show();

	return Status::Success;
}

auto MyApp::OnUpdate(F64 deltaTime) -> void
{
}

auto MyApp::OnShutdown() -> Status
{
	return Status::Success;
}

GAZE_REGISTER_APP(MyApp);

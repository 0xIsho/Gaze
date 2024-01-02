#include "Client/App.hpp"

#include "Core/Type.hpp"
#include "WM/Window.hpp"
#include "Input/Input.hpp"
#include "Events/Dispatcher.hpp"
#include "Events/WindowEvent.hpp"

using namespace Gaze;

class MyApp : public Gaze::Client::App
{
public:
	MyApp(int argc, char** argv);

private:
		auto OnInit() -> Status override;
		auto OnUpdate(F64 deltaTime) -> void override;
		auto OnShutdown() -> Status override;

private:
	Gaze::Mem::Shared<Gaze::WM::Window> m_Win;
	Gaze::Input::Handler m_Input;
};

MyApp::MyApp(int argc, char** argv)
	: App(argc, argv)
	, m_Win(Gaze::Mem::MakeShared<Gaze::WM::Window>("Hello Input", 800, 600))
	, m_Input(m_Win)
{
}

auto MyApp::OnInit() -> Status
{
	m_Win->OnEvent([this](auto& event) {
		Events::Dispatcher(event).Dispatch<Events::WindowClose>([this](auto&) {
			Quit();
		});
	});

	m_Win->Show();

	return App::Status::Success;
}

auto MyApp::OnUpdate(F64 deltaTime) -> void
{
	if (m_Input.IsKeyPressed(Gaze::Input::Key::kEscape)) {
		Quit();
	}
}

auto MyApp::OnShutdown() -> Status
{
	return App::Status::Success;
}

GAZE_REGISTER_APP(MyApp);

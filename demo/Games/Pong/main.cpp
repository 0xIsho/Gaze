#include "Client/App.hpp"

#include "Core/Type.hpp"

#include "WM/Window.hpp"

using namespace Gaze;

class MyApp : public Client::App
{
public:
	MyApp(int argc, char** argv);

private:
	auto OnInit() -> Status;
	auto OnUpdate(F64 deltaTime) -> void;
	auto OnShutdown() -> Status;

private:
	Mem::Shared<WM::Window> m_Win;
};

MyApp::MyApp(int argc, char** argv)
	: App(argc, argv)
	, m_Win(Mem::MakeShared<WM::Window>("Gaze - Pong", 864, 486))
{
}

auto MyApp::OnInit() -> Status
{
	m_Win->OnClose([this] {
		Quit();
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

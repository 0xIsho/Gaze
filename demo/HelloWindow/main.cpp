#include "Client/App.hpp"

#include "WM/Window.hpp"

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
	Gaze::WM::Window m_Win;
};

MyApp::MyApp(int argc, char** argv)
	: App(argc, argv)
	, m_Win("Hello Window", 800, 600)
{
}

auto MyApp::OnInit() -> Status
{
	m_Win.OnClose([this] {
		Quit();
	});

	m_Win.Show();

	return App::Status::Success;
}

auto MyApp::OnUpdate(F64 deltaTime) -> void
{
}

auto MyApp::OnShutdown() -> Status
{
	return App::Status::Success;
}

GAZE_REGISTER_APP(MyApp);

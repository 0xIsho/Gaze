#include "Client/App.hpp"

#include "WM/Window.hpp"

#include "Events/WindowEvent.hpp"
#include "Events/Dispatcher.hpp"

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
	WM::Window m_Win;
};

MyApp::MyApp(int argc, char** argv)
	: ClientApp(argc, argv)
	, m_Win("Hello Window", 800, 600)
{
}

auto MyApp::OnInit() -> Status
{
	m_Win.OnEvent([this](auto& event) {
		Events::Dispatcher(event).Dispatch<Events::WindowClose>([this](auto&) {
			Quit();
		});
	});

	m_Win.Show();

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

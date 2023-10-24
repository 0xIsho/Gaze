#include "Client/App.hpp"

#include "WM/Core.hpp"
#include "WM/Window.hpp"

class MyApp : public Gaze::Client::App
{
public:
	MyApp(int argc, char** argv);

private:
	auto OnInit() -> Status override;
	auto OnUpdate() -> void override;
	auto OnShutdown() -> Status override;

private:
	Gaze::WM::Window m_Win;
};

MyApp::MyApp(int argc, char** argv)
	: App(argc, argv)
	, m_Win("Sandbox", 1024, 768)
{
	m_Win.OnClose([this] {
		Quit();
	});
}

auto MyApp::OnInit() -> Status
{
	if (!Gaze::WM::Init()) {
		return Status::Fail;
	}

	if (!m_Win.Show()) {
		return Status::Fail;
	}

	return Status::Success;
}

auto MyApp::OnUpdate() -> void
{
	Gaze::WM::PollEvents();
	m_Win.SwapBuffers();
}

auto MyApp::OnShutdown() -> Status
{
	Gaze::WM::Terminate();

	return Status::Success;
}

auto main(int argc, char** argv) -> int
{
	auto app = MyApp(argc, argv);
	return app.Run() == Gaze::Client::App::Status::Success ? 0 : 1;
}

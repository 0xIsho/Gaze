#include "Client/App.hpp"

#include "Core/Type.hpp"

#include "WM/Core.hpp"
#include "WM/Window.hpp"

#include "GFX/Renderer.hpp"

class MyApp : public Gaze::Client::App
{
public:
	MyApp(int argc, char** argv);

private:
	auto OnInit() -> Status override;
	auto OnUpdate() -> void override;
	auto OnShutdown() -> Status override;

private:
	Gaze::Mem::Shared<Gaze::WM::Window> m_Win;
	Gaze::Mem::Unique<Gaze::GFX::Renderer> m_Rdr;
};

MyApp::MyApp(int argc, char** argv)
	: App(argc, argv)
	, m_Win(Gaze::Mem::MakeShared<Gaze::WM::Window>("Sandbox", 1280, 640))
{
	m_Rdr = Gaze::GFX::CreateRenderer(m_Win);

	m_Win->OnClose([this] {
		Quit();
	});
}

auto MyApp::OnInit() -> Status
{
	m_Win->Show();

	m_Rdr->SetClearColor(.8F, .8F, .8F, 1.0F);

	return Status::Success;
}

auto MyApp::OnUpdate() -> void
{
	m_Rdr->Clear();

	for (auto x = 0.0F; x < 1.0F; x += 0.01F) {
		for (auto y = 0.0F; y < 1.0F; y += 0.01F) {
			m_Rdr->DrawPoint(x, y);
		}
	}

	m_Rdr->Render();
}

auto MyApp::OnShutdown() -> Status
{
	return Status::Success;
}

GAZE_REGISTER_APP(MyApp);

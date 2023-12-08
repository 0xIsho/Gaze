#include "Client/App.hpp"

#include "Core/Type.hpp"
#include "WM/Window.hpp"
#include "GFX/Renderer.hpp"

using namespace Gaze;

class MyApp : public Gaze::Client::App
{
public:
	MyApp(int argc, char** argv);

private:
		auto OnInit() -> Status override;
		auto OnUpdate(F64 deltaTime) -> void override;
		auto OnShutdown() -> Status override;

		auto DrawAxis() -> void;
private:
	Gaze::Mem::Shared<Gaze::WM::Window> m_Win;
	Gaze::Mem::Shared<Gaze::GFX::Renderer> m_Rdr;
};

MyApp::MyApp(int argc, char** argv)
	: App(argc, argv)
	, m_Win(Gaze::Mem::MakeShared<Gaze::WM::Window>("Hello Viewport", 800, 600))
	, m_Rdr(Gaze::GFX::CreateRenderer(m_Win))
{
}

auto MyApp::OnInit() -> Status
{
	m_Win->OnClose([this] {
		Quit();
	});
	m_Win->Show();

	m_Rdr->SetClearColor(.8F, .8F, .8F, 1.F);
	m_Rdr->Clear();
	m_Rdr->SetClearColor(.2F, .2F, .2F, 1.F);

	return App::Status::Success;
}

auto MyApp::OnUpdate(F64 deltaTime) -> void
{
	m_Rdr->SetViewport(50, 150, 300, 300);
	m_Rdr->Clear();
	DrawAxis();

	m_Rdr->DrawTri({{
		{ -.5F,  .5F, 0.F },
		{ -.5F, -.5F, 0.F },
		{  .5F, -.5F, 0.F }
	}});
	m_Rdr->FillTri({{
		{  .5F,  .5F, 0.F },
		{ -.5F,  .5F, 0.F },
		{  .5F, -.5F, 0.F }
	}});

	m_Rdr->SetViewport(450, 150, 300, 300);
	m_Rdr->Clear();
	DrawAxis();

	m_Rdr->DrawTri({{
		{ -.5F,  .5F, 0.F },
		{  .5F, -.5F, 0.F },
		{  .5F,  .5F, 0.F }
	}});
	m_Rdr->FillTri({{
		{ -.5F,  .5F, 0.F },
		{ -.5F, -.5F, 0.F },
		{  .5F, -.5F, 0.F }
	}});

	m_Rdr->Render();
}

auto MyApp::OnShutdown() -> Status
{
	return App::Status::Success;
}

auto MyApp::DrawAxis() -> void
{
	m_Rdr->DrawLine({ -1.F,  0.F, 0.F }, { 1.F, 0.F, 0.F });
	m_Rdr->DrawLine({  0.F, -1.F, 0.F }, { 0.F, 1.F, 0.F });
}

GAZE_REGISTER_APP(MyApp);

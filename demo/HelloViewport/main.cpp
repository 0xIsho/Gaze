#include "Client/App.hpp"

#include "Core/Type.hpp"
#include "WM/Window.hpp"
#include "GFX/Renderer.hpp"
#include "GFX/Primitives.hpp"
#include "Events/Dispatcher.hpp"
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

	auto DrawAxis() -> void;

private:
	Mem::Shared<WM::Window> m_Win;
	Mem::Shared<GFX::Renderer> m_Rdr;
};

MyApp::MyApp(int argc, char** argv)
	: ClientApp(argc, argv)
	, m_Win(Mem::MakeShared<WM::Window>("Hello Viewport", 800, 600))
	, m_Rdr(GFX::CreateRenderer(m_Win))
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

	m_Rdr->SetClearColor(.2F, .2F, .2F, 1.F);

	return Status::Success;
}

auto MyApp::OnUpdate(F64 deltaTime) -> void
{
	m_Rdr->Clear();

	m_Rdr->SetViewport(50, 150, 300, 300);
	DrawAxis();

	const auto tri1 = GFX::CreateTriangle({{
		{ -.5F,  .5F, 0.F },
		{ -.5F, -.5F, 0.F },
		{  .5F, -.5F, 0.F }
	}});
	const auto tri2 = GFX::CreateTriangle({{
		{  .5F,  .5F, 0.F },
		{ -.5F,  .5F, 0.F },
		{  .5F, -.5F, 0.F }
	}});

	m_Rdr->DrawMesh(tri1, GFX::Renderer::PrimitiveMode::LineLoop);
	m_Rdr->DrawMesh(tri2, GFX::Renderer::PrimitiveMode::Triangles);

	m_Rdr->Flush();

	m_Rdr->SetViewport(450, 150, 300, 300);
	DrawAxis();

	m_Rdr->DrawMesh(tri1, GFX::Renderer::PrimitiveMode::Triangles);
	m_Rdr->DrawMesh(tri2, GFX::Renderer::PrimitiveMode::LineLoop);

	m_Rdr->Render();
}

auto MyApp::OnShutdown() -> Status
{
	return Status::Success;
}

auto MyApp::DrawAxis() -> void
{
	m_Rdr->DrawMesh(GFX::CreateLine({ -1.F,  0.F, 0.F }, { 1.F, 0.F, 0.F }), GFX::Renderer::PrimitiveMode::Lines);
	m_Rdr->DrawMesh(GFX::CreateLine({  0.F, -1.F, 0.F }, { 0.F, 1.F, 0.F }), GFX::Renderer::PrimitiveMode::Lines);
}

GAZE_REGISTER_APP(MyApp);

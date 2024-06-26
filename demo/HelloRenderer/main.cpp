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

private:
	Shared<WM::Window> m_Win;
	Shared<GFX::Renderer> m_Rdr;
};

MyApp::MyApp(int argc, char** argv)
	: ClientApp(argc, argv)
	, m_Win(MakeShared<WM::Window>("Hello Renderer", 800, 600))
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

	return Status::Success;
}

auto MyApp::OnUpdate(F64 deltaTime) -> void
{
	m_Rdr->Clear();

	m_Rdr->DrawMesh(GFX::CreateLine({ -1.F,  0.F, 0.F }, { 1.F, 0.F, 0.F }), GFX::Renderer::PrimitiveMode::Lines);
	m_Rdr->DrawMesh(GFX::CreateLine({  0.F, -1.F, 0.F }, { 0.F, 1.F, 0.F }), GFX::Renderer::PrimitiveMode::Lines);

	m_Rdr->DrawMesh(GFX::CreateTriangle({{
		{ -.5F,  .5F, 0.F },
		{ -.5F, -.5F, 0.F },
		{  .5F, -.5F, 0.F }
	}}), GFX::Renderer::PrimitiveMode::LineLoop);
	m_Rdr->DrawMesh(GFX::CreateTriangle({{
		{  .5F,  .5F, 0.F },
		{ -.5F,  .5F, 0.F },
		{  .5F, -.5F, 0.F }
	}}), GFX::Renderer::PrimitiveMode::Triangles);

	for (auto i = -1.F; i <= 1.F; i += .1F) {
		for (auto j = -1.F; j <= 1.F; j += .1F) {
			m_Rdr->DrawMesh(GFX::CreatePoint({ i, j, 0.F }), GFX::Renderer::PrimitiveMode::Points);
		}
	}

	m_Rdr->Render();
}

auto MyApp::OnShutdown() -> Status
{
	return Status::Success;
}

GAZE_REGISTER_APP(MyApp);

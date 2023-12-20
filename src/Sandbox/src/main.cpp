#include "Client/App.hpp"

#include "Core/Type.hpp"

#include "WM/Core.hpp"
#include "WM/Window.hpp"

#include "GFX/Camera.hpp"
#include "GFX/Renderer.hpp"

#include "Input/Input.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <iomanip>

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
	Gaze::Mem::Unique<Gaze::GFX::Renderer> m_Rdr;
	Gaze::Input::Handler m_Input;
	Gaze::Mem::Shared<Gaze::GFX::Camera> m_Cam;
};

MyApp::MyApp(int argc, char** argv)
	: App(argc, argv)
	, m_Win(Gaze::Mem::MakeShared<Gaze::WM::Window>("Sandbox", 1280, 640))
	, m_Input(m_Win)
	, m_Cam(Mem::MakeShared<Gaze::GFX::Camera>())
{
	m_Rdr = Gaze::GFX::CreateRenderer(m_Win);
	m_Rdr->Clear();

	m_Win->OnClose([this] {
		Quit();
	});
}

auto MyApp::OnInit() -> Status
{
	m_Win->Show();

	m_Rdr->SetProjection(glm::perspective(glm::radians(75.F), F32(m_Win->Width()) / F32(m_Win->Height()), .1F, 10.F));
	m_Rdr->SetCamera(m_Cam);

	return Status::Success;
}

auto MyApp::OnUpdate(F64 deltaTime) -> void
{
	m_Rdr->Clear();

	if (m_Input.IsKeyPressed(Gaze::Input::Key::kEscape)) {
		Quit();
	}

	auto mesh = Gaze::GFX::Mesh{
		{
			{  .25F, -.25F, .0F },
			{  .25F,  .25F,  .0F },
			{  .0F,   .5F,  .0F },
			{ -.25F,  .25F,  .0F },
			{ -.25F, -.25F, .0F },
			{  .0F,  -.5F,  .0F },
		},
		{ 0, 1, 2, 3, 4, 5 }
	};

	m_Rdr->SetColor(1.F, 1.F, 1.F, 1.F);
	m_Rdr->DrawLine({ -1, 0, 0 }, { 1, 0, 0 });
	m_Rdr->DrawLine({ 0, -1, 0 }, { 0, 1, 0 });

	for (auto i = -1.F; i <= 1.F; i += .1F) {
		m_Rdr->SetColor(.5F, .2F, 1.F, 1.F);
		m_Rdr->DrawLine({ i, -.0075F, 0 }, { i, .0075F, 0 });
		m_Rdr->DrawLine({ -.0075F, i, 0 }, { .0075F, i, 0 });

		for (auto j = -1.F; j <= 1.F; j += .1F) {
			m_Rdr->DrawPoint({ i, j, 0.0F });
		}
	}

	m_Rdr->SetColor(1.F, .0F, .0F, 1.F);
	m_Rdr->DrawMesh(mesh, Gaze::GFX::Renderer::PrimitiveMode::LineLoop);

	m_Rdr->SetColor(1.F, 1.F, 0.F, 1.F);
	m_Rdr->DrawTri({{ { -.6F,  .4F, 0 }, { -.5F,  .6F, 0 }, { -.4F,  .4F, 0 } }});
	m_Rdr->FillTri({{ { -.6F, -.4F, 0 }, { -.5F, -.6F, 0 }, { -.4F, -.4F, 0 } }});

	m_Rdr->DrawTri({{ { -.1F,  .1F, 0 }, { -.1F, -.1F, 0 }, {  .1F, -.1F, 0 } }});
	m_Rdr->FillTri({{ {  .1F, -.1F, 0 }, {  .1F,  .1F, 0 }, { -.1F,  .1F, 0 } }});

	m_Rdr->DrawTri({{ {  .6F, -.4F, 0 }, {  .4F, -.4F, 0 }, {  .5F, -.6F, 0 } }});
	m_Rdr->FillTri({{ {  .6F,  .4F, 0 }, {  .5F,  .6F, 0 }, {  .4F,  .4F, 0 } }});

	m_Rdr->Render();

	std::cout
		<< "Delta Time: "
		<< std::fixed << std::setprecision(4) << deltaTime
		<< " | FPS: " << I32(1 / deltaTime) << '\n';
}

auto MyApp::OnShutdown() -> Status
{
	return Status::Success;
}

GAZE_REGISTER_APP(MyApp);

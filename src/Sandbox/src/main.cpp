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
	Gaze::GFX::Camera m_Cam;
};

MyApp::MyApp(int argc, char** argv)
	: App(argc, argv)
	, m_Win(Gaze::Mem::MakeShared<Gaze::WM::Window>("Sandbox", 1280, 640))
	, m_Input(m_Win)
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

	mesh.Rotate(glm::radians(30.0F), { .0F, 1.F, 1.F });
	mesh.Scale({ .5F, .5F, .5F });

	const auto projection = glm::perspective(glm::radians(75.F), F32(m_Win->Width()) / F32(m_Win->Height()), .1F, 10.F);
	const auto view = m_Cam.ComputeViewMatrix();
	const auto mvp = projection * view * mesh.Transform();

	m_Rdr->SetColor(1.F, 1.F, 1.F, 1.F);
	m_Rdr->DrawLine(glm::vec4{ -1, 0, 0, 1 } * mvp, glm::vec4{ 1, 0, 0, 1 } * mvp);
	m_Rdr->DrawLine(glm::vec4{ 0, -1, 0, 1 } * mvp, glm::vec4{ 0, 1, 0, 1 } * mvp);

	for (auto i = -1.F; i <= 1.F; i += .1F) {
		m_Rdr->SetColor(.5F, .2F, 1.F, 1.F);
		m_Rdr->DrawLine(glm::vec4{ i, -.0075F, 0, 1 } * mvp, glm::vec4{ i, .0075F, 0, 1 } * mvp);
		m_Rdr->DrawLine(glm::vec4{ -.0075F, i, 0, 1 } * mvp, glm::vec4{ .0075F, i, 0, 1 } * mvp);

		for (auto j = -1.F; j <= 1.F; j += .1F) {
			m_Rdr->DrawPoint(glm::vec4{ i, j, 0.0F, 1.0F } * mvp);
		}
	}

	m_Rdr->SetColor(1.F, .0F, .0F, 1.F);
	m_Rdr->DrawMesh(mesh, Gaze::GFX::Renderer::PrimitiveMode::LineLoop);

	m_Rdr->SetColor(1.F, 1.F, 0.F, 1.F);
	m_Rdr->DrawTri({{ glm::vec4{ -.6F, .4F , 0, 1 } * mvp,glm::vec4 { -.5F, .6F, 0, 1 } * mvp, glm::vec4{ -.4F, .4F, 0, 1 } * mvp }});
	m_Rdr->FillTri({{ glm::vec4{ -.6F, -.4F , 0, 1 } * mvp,glm::vec4 { -.5F, -.6F, 0, 1 } * mvp, glm::vec4{ -.4F, -.4F, 0, 1 } * mvp }});

	m_Rdr->DrawTri({{ glm::vec4{ -.1F, .1F, 0, 1 } * mvp, glm::vec4{ -.1F, -.1F, 0, 1 } * mvp, glm::vec4{ .1F, -.1F, 0, 1 } * mvp }});
	m_Rdr->FillTri({{ glm::vec4{ .1F, -.1F, 0, 1 } * mvp, glm::vec4{ .1F, .1F, 0, 1 } * mvp, glm::vec4{ -.1F, .1F, 0, 1 } * mvp }});

	m_Rdr->DrawTri({{ glm::vec4{ .6F, -.4F, 0, 1 } * mvp, glm::vec4{ .4F, -.4F, 0, 1 } * mvp, glm::vec4{ .5F, -.6F, 0, 1 } * mvp }});
	m_Rdr->FillTri({{ glm::vec4{ .6F, .4F, 0, 1 } * mvp, glm::vec4{ .5F, .6F, 0, 1 } * mvp, glm::vec4{ .4F, .4F, 0, 1 } * mvp }});

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

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

	double m_Yaw{};
	double m_Pitch{};
};

MyApp::MyApp(int argc, char** argv)
	: App(argc, argv)
	, m_Win(Gaze::Mem::MakeShared<Gaze::WM::Window>("Sandbox", 1280, 640))
	, m_Input(m_Win)
	, m_Cam(Mem::MakeShared<Gaze::GFX::Camera>())
{
	m_Cam->SetPosition({ 1.F, 1.F, 1.F });
	m_Cam->SetFront({ -1.F, -1.F, -1.F });

	m_Rdr = Gaze::GFX::CreateRenderer(m_Win);
	m_Rdr->Clear();

	m_Win->OnClose([this] {
		Quit();
	});

	m_Win->OnMouseMove([this] (auto x, auto y) {
		static auto lastX = 1280.0 / 2;
		static auto lastY = 640.0 / 2;
		constexpr auto sensitivity = .5;

		const auto deltaX = (x - lastX) * sensitivity;
		const auto deltaY = (lastY - y) * sensitivity;

		m_Yaw += deltaX;
		m_Pitch += deltaY;

		if (m_Pitch > 89) {
			m_Pitch = 89;
		}
		if (m_Pitch < -89) {
			m_Pitch = -89;
		}

		glm::vec3 direction;
		direction.x = F32(cos(glm::radians(m_Yaw))) * F32(cos(glm::radians(m_Pitch)));
		direction.y = F32(sin(glm::radians(m_Pitch)));
		direction.z = F32(sin(glm::radians(m_Yaw))) * F32(cos(glm::radians(m_Pitch)));
		m_Cam->SetFront(glm::normalize(direction));

		lastX = x;
		lastY = y;
	});
}

auto MyApp::OnInit() -> Status
{
	m_Win->Show();

	m_Rdr->SetProjection(glm::perspective(glm::radians(75.F), F32(m_Win->Width()) / F32(m_Win->Height()), .1F, 100.F));
	m_Rdr->SetCamera(m_Cam);
	m_Rdr->SetClearColor(.1F, .1F, .1F, 1.F);

	return Status::Success;
}

auto MyApp::OnUpdate(F64 deltaTime) -> void
{
	constexpr auto cameraSpeed = 0.5F; // adjust accordingly
	if (m_Input.IsKeyPressed(Input::Key::kW)) {
		m_Cam->Move(cameraSpeed * F32(deltaTime) * m_Cam->Front());
	}
	if (m_Input.IsKeyPressed(Input::Key::kS)) {
		m_Cam->Move(cameraSpeed * F32(deltaTime) * -m_Cam->Front());
	}
	if (m_Input.IsKeyPressed(Input::Key::kE)) {
		m_Cam->Move(cameraSpeed * F32(deltaTime) * m_Cam->Up());
	}
	if (m_Input.IsKeyPressed(Input::Key::kQ)) {
		m_Cam->Move(cameraSpeed * F32(deltaTime) * -m_Cam->Up());
	}
	if (m_Input.IsKeyPressed(Input::Key::kA)) {
		m_Cam->Move(cameraSpeed * F32(deltaTime) * -glm::normalize(glm::cross(m_Cam->Front(), m_Cam->Up())));
	}
	if (m_Input.IsKeyPressed(Input::Key::kD)) {
		m_Cam->Move(cameraSpeed * F32(deltaTime) * glm::normalize(glm::cross(m_Cam->Front(), m_Cam->Up())));
	}

	m_Rdr->Clear();

	if (m_Input.IsKeyPressed(Gaze::Input::Key::kEscape)) {
		Quit();
	}

	auto mesh1 = Gaze::GFX::Mesh{
		{
			{ { -.25F, -.25F, .0F } },
			{ { -.25F,  .25F, .0F } },
			{ {  -.5F,   .5F, .0F } },
			{ { -.75F,  .25F, .0F } },
			{ { -.75F, -.25F, .0F } },
			{ {  -.5F,  -.5F, .0F } },
		},
		{
			0, 1,
			1, 2,
			2, 3,
			3, 4,
			4, 5
		}
	};

	auto mesh2 = Gaze::GFX::Mesh{
		{
			{ {  .25F, -.25F, .0F } },
			{ {  .25F,  .25F, .0F } },
			{ {   .0F,   .5F, .0F } },
			{ { -.25F,  .25F, .0F } },
			{ { -.25F, -.25F, .0F } },
			{ {   .0F,  -.5F, .0F } },
		},
		{
			0, 1, 3,
			1, 2, 3,
			3, 4, 0,
			4, 5, 0,
		}
	};

	auto mesh3 = Gaze::GFX::Mesh{
		{
			{ { .25F, -.25F, .0F } },
			{ { .25F,  .25F, .0F } },
			{ {  .5F,   .5F, .0F } },
			{ { .75F,  .25F, .0F } },
			{ { .75F, -.25F, .0F } },
			{ {  .5F,  -.5F, .0F } },
		},
		{
			0, 1,
			1, 2,
			2, 3,
			3, 4,
			4, 5
		}
	};

	m_Rdr->DrawMesh(mesh1, GFX::Renderer::PrimitiveMode::LineLoop);
	m_Rdr->DrawMesh(mesh2, GFX::Renderer::PrimitiveMode::Triangles);
	m_Rdr->DrawMesh(mesh3, GFX::Renderer::PrimitiveMode::LineLoop);

	for (auto i = -.9F; i <= 1.F; i += .1F) {
		m_Rdr->DrawLine({ i, .0F, -1.F }, { i, .0F, 1.F });
		m_Rdr->DrawLine({ -1.F, .0F, i }, { 1.F, .0F, i });
	}

	m_Rdr->DrawLine({ -1, 0, 0 }, { 1, 0, 0 });
	m_Rdr->DrawLine({ 0, -1, 0 }, { 0, 1, 0 });
	m_Rdr->DrawLine({ 0, 0, -1 }, { 0, 0, 1 });

	m_Rdr->Render();
}

auto MyApp::OnShutdown() -> Status
{
	return Status::Success;
}

GAZE_REGISTER_APP(MyApp);

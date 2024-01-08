#include "Client/App.hpp"

#include "Core/Type.hpp"

#include "Events/Dispatcher.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Events/WindowEvent.hpp"

#include "Input/KeyCode.hpp"

#include "WM/Core.hpp"
#include "WM/Window.hpp"

#include "GFX/Camera.hpp"
#include "GFX/Renderer.hpp"
#include "GFX/Primitives.hpp"

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
	auto OnFixedUpdate(F64 deltaTime) -> void override;
	auto OnShutdown() -> Status override;

private:
	Gaze::Mem::Shared<Gaze::WM::Window> m_Win;
	Gaze::Mem::Unique<Gaze::GFX::Renderer> m_Rdr;
	Gaze::Mem::Shared<Gaze::GFX::Camera> m_Cam;

	glm::vec3 m_CameraDir{};

	double m_Yaw{};
	double m_Pitch{};
};

MyApp::MyApp(int argc, char** argv)
	: App(argc, argv)
	, m_Win(Gaze::Mem::MakeShared<Gaze::WM::Window>("Sandbox", 1280, 640))
	, m_Cam(Mem::MakeShared<Gaze::GFX::Camera>())
{
	m_Cam->SetPosition({ 1.F, 1.F, 1.F });
	m_Cam->SetFront({ -1.F, -1.F, -1.F });

	m_Rdr = Gaze::GFX::CreateRenderer(m_Win);
	m_Rdr->Clear();
}

auto MyApp::OnInit() -> Status
{
	m_Win->OnEvent([this](auto& event) {
		auto dispatcher = Events::Dispatcher(event);

		dispatcher.Dispatch<Events::WindowClose>([this](auto&) {
			Quit();
		});

		dispatcher.Dispatch<Events::MouseMove>([this](auto& event) {
			static auto lastX = 1280.0 / 2;
			static auto lastY = 640.0 / 2;
			constexpr auto sensitivity = .5;

			const auto deltaX = (event.X() - lastX) * sensitivity;
			const auto deltaY = (lastY - event.Y()) * sensitivity;

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

			lastX = event.X();
			lastY = event.Y();
		});

		dispatcher.Dispatch<Events::KeyPressed>([this](auto& event) {
			if (event.Keycode() == Input::Key::kW) {
				m_CameraDir.z += 1;
			}
			if (event.Keycode() == Input::Key::kS) {
				m_CameraDir.z -= 1;
			}
			if (event.Keycode() == Input::Key::kE) {
				m_CameraDir.y += 1;
			}
			if (event.Keycode() == Input::Key::kQ) {
				m_CameraDir.y -= 1;
			}
			if (event.Keycode() == Input::Key::kD) {
				m_CameraDir.x += 1;
			}
			if (event.Keycode() == Input::Key::kA) {
				m_CameraDir.x -= 1;
			}
		});

		dispatcher.Dispatch<Events::KeyReleased>([this](auto& event) {
			if (event.Keycode() == Input::Key::kEscape) {
				Quit();
			}

			if (event.Keycode() == Input::Key::kW) {
				m_CameraDir.z -= 1;
			}
			if (event.Keycode() == Input::Key::kS) {
				m_CameraDir.z += 1;
			}
			if (event.Keycode() == Input::Key::kE) {
				m_CameraDir.y -= 1;
			}
			if (event.Keycode() == Input::Key::kQ) {
				m_CameraDir.y += 1;
			}
			if (event.Keycode() == Input::Key::kD) {
				m_CameraDir.x -= 1;
			}
			if (event.Keycode() == Input::Key::kA) {
				m_CameraDir.x += 1;
			}
		});

	});

	m_Win->Show();

	m_Rdr->SetProjection(glm::perspective(glm::radians(75.F), F32(m_Win->Width()) / F32(m_Win->Height()), .1F, 100.F));
	m_Rdr->SetCamera(m_Cam);
	m_Rdr->SetClearColor(.1F, .1F, .1F, 1.F);

	return Status::Success;
}

auto MyApp::OnUpdate(F64 /*deltaTime*/) -> void
{
	m_Rdr->Clear();

	const auto greenMat = GFX::Material{
		{ 0.F, 1.F, 0.F },
		{ .5F, .5F, .5F },
		32.F
	};

	auto cube = GFX::Mesh{
		{
			// Back-Front
			{{  0.5F, 1.0F,  0.5F }, { 0.0F, 0.0F,  1.0F }},
			{{ -0.5F, 1.0F,  0.5F }, { 0.0F, 0.0F,  1.0F }},
			{{ -0.5F, 0.0F,  0.5F }, { 0.0F, 0.0F,  1.0F }},
			{{  0.5F, 0.0F,  0.5F }, { 0.0F, 0.0F,  1.0F }},
			{{ -0.5F, 1.0F, -0.5F }, { 0.0F, 0.0F, -1.0F }},
			{{  0.5F, 1.0F, -0.5F }, { 0.0F, 0.0F, -1.0F }},
			{{  0.5F, 0.0F, -0.5F }, { 0.0F, 0.0F, -1.0F }},
			{{ -0.5F, 0.0F, -0.5F }, { 0.0F, 0.0F, -1.0F }},
			// Left-Right
			{{  0.5F, 1.0F,  0.5F }, {  1.0F, 0.0F, 0.0F }},
			{{ -0.5F, 1.0F,  0.5F }, { -1.0F, 0.0F, 0.0F }},
			{{ -0.5F, 0.0F,  0.5F }, { -1.0F, 0.0F, 0.0F }},
			{{  0.5F, 0.0F,  0.5F }, {  1.0F, 0.0F, 0.0F }},
			{{ -0.5F, 1.0F, -0.5F }, { -1.0F, 0.0F, 0.0F }},
			{{  0.5F, 1.0F, -0.5F }, {  1.0F, 0.0F, 0.0F }},
			{{  0.5F, 0.0F, -0.5F }, {  1.0F, 0.0F, 0.0F }},
			{{ -0.5F, 0.0F, -0.5F }, { -1.0F, 0.0F, 0.0F }},
			// Bottom-Top
			{{  0.5F, 1.0F,  0.5F }, { 0.0F,  1.0F, 0.0F }},
			{{ -0.5F, 1.0F,  0.5F }, { 0.0F,  1.0F, 0.0F }},
			{{ -0.5F, 0.0F,  0.5F }, { 0.0F, -1.0F, 0.0F }},
			{{  0.5F, 0.0F,  0.5F }, { 0.0F, -1.0F, 0.0F }},
			{{ -0.5F, 1.0F, -0.5F }, { 0.0F,  1.0F, 0.0F }},
			{{  0.5F, 1.0F, -0.5F }, { 0.0F,  1.0F, 0.0F }},
			{{  0.5F, 0.0F, -0.5F }, { 0.0F, -1.0F, 0.0F }},
			{{ -0.5F, 0.0F, -0.5F }, { 0.0F, -1.0F, 0.0F }},
		},
		{
			// Front
			0, 1, 2,
			2, 3, 0,
			// Back
			4, 5, 6,
			6, 7, 4,
			// Left
			1 + 8, 4 + 8, 7 + 8,
			7 + 8, 2 + 8, 1 + 8,
			// Right
			0 + 8, 3 + 8, 6 + 8,
			6 + 8, 5 + 8, 0 + 8,
			// Top
			0 + 16, 5 + 16, 4 + 16,
			4 + 16, 1 + 16, 0 + 16,
			// Bottom
			2 + 16, 7 + 16, 6 + 16,
			6 + 16, 3 + 16, 2 + 16
		}
	};
	cube.SetMaterial(greenMat);

	m_Rdr->DrawMesh(cube, GFX::Renderer::PrimitiveMode::Triangles);

	m_Rdr->DrawMesh(GFX::CreateTriangle({{
		{  5.0F, 3.0F,  0.0F },
		{  0.0F, 3.0F, -5.0F },
		{ -5.0F, 3.0F,  5.0F },
	}}), GFX::Renderer::PrimitiveMode::Triangles);
	m_Rdr->DrawMesh(GFX::CreateTriangle({{
		{  5.0F, -3.0F,  0.0F },
		{  0.0F, -3.0F, -5.0F },
		{ -5.0F, -3.0F,  5.0F },
	}}), GFX::Renderer::PrimitiveMode::Triangles);

	auto plane = GFX::CreateQuad({ 0.0F, -1.0F, 0.0F }, 10, 10);
	m_Rdr->DrawMesh(plane, GFX::Renderer::PrimitiveMode::Triangles);
	plane.SetPosition({ 0.0F, 1.0F, 0.0F });
	m_Rdr->DrawMesh(plane, GFX::Renderer::PrimitiveMode::Triangles);

	for (auto i = -10.F; i <= 10.F; i += .5F) {
		m_Rdr->DrawMesh(GFX::CreateLine({ i, .0F, -10.F }, { i, .0F, 10.F }), GFX::Renderer::PrimitiveMode::Lines);
		m_Rdr->DrawMesh(GFX::CreateLine({ -10.F, .0F, i }, { 10.F, .0F, i }), GFX::Renderer::PrimitiveMode::Lines);
	}

	m_Rdr->DrawMesh(GFX::CreateLine({ -10, 0, 0 }, { 10, 0, 0 }), GFX::Renderer::PrimitiveMode::Lines);
	m_Rdr->DrawMesh(GFX::CreateLine({ 0, -10, 0 }, { 0, 10, 0 }), GFX::Renderer::PrimitiveMode::Lines);
	m_Rdr->DrawMesh(GFX::CreateLine({ 0, 0, -10 }, { 0, 0, 10 }), GFX::Renderer::PrimitiveMode::Lines);

	m_Rdr->Render();
}

auto MyApp::OnFixedUpdate(F64 deltaTime) -> void
{
	constexpr auto cameraSpeed = 2.0F;
	m_Cam->Move(m_Cam->Front()  * m_CameraDir.z * cameraSpeed * F32(deltaTime));
	m_Cam->Move(m_Cam->Up()     * m_CameraDir.y * cameraSpeed * F32(deltaTime));
	m_Cam->Move(m_Cam->Right()  * m_CameraDir.x * cameraSpeed * F32(deltaTime));
}

auto MyApp::OnShutdown() -> Status
{
	return Status::Success;
}

GAZE_REGISTER_APP(MyApp);

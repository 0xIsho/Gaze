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
#include "GFX/Light.hpp"
#include "GFX/Renderer.hpp"
#include "GFX/Primitives.hpp"

#include "Physics/World.hpp"
#include "Physics/Shape.hpp"
#include "Physics/Rigidbody.hpp"

#include "IO/Loader/Scene.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

using namespace Gaze;

class MyApp : public Client::ClientApp
{
public:
	MyApp(int argc, char** argv);

private:
	auto OnInit() -> Status override;
	auto OnUpdate(F64 deltaTime) -> void override;
	auto OnFixedUpdate(F64 deltaTime) -> void override;
	auto OnShutdown() -> Status override;
	auto OnPacketReceived(U32 sender, Net::Packet packet) -> void override;

private:
	Gaze::Unique<Gaze::GFX::Renderer> m_Rdr;
	Shared<Gaze::WM::Window> m_Win;
	Shared<Gaze::GFX::Camera> m_Cam;

	glm::vec3 m_CameraDir{};

	double m_Yaw{};
	double m_Pitch{};

	Physics::World m_PhysicsWorld;
	Shared<Physics::Rigidbody> m_RbCube;
	std::vector<GFX::Object> m_Objects;
};

MyApp::MyApp(int argc, char** argv)
	: ClientApp(argc, argv)
	, m_Win(Gaze::MakeShared<Gaze::WM::Window>("Sandbox", 1280, 640))
	, m_Cam(MakeShared<Gaze::GFX::PerspectiveCamera>(
		glm::radians(75.F),
		F32(m_Win->Width()) / F32(m_Win->Height()),
		.1F,
		100.F
	))
{
	m_Cam->SetPosition({ 1.F, 1.F, 1.F });
	m_Cam->SetFront({ -1.F, -1.F, -1.F });

	m_Rdr = Gaze::GFX::CreateRenderer(m_Win);
	m_Rdr->Clear();

	m_PhysicsWorld.AddRigidbody(MakeShared<Physics::Rigidbody>(MakeShared<Physics::BoxShape>(10.F, .0002F, 10.F), 0.F));
	m_RbCube = MakeShared<Physics::Rigidbody>(MakeShared<Physics::BoxShape>(1.F, 1.F, 1.F));
	m_RbCube->SetOrigin(0, 10, 0);
	m_RbCube->SetRotation({ 1, 1, 1 }, glm::radians(45.F));

	m_PhysicsWorld.AddRigidbody(m_RbCube);
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
			if (event.Keycode() == Input::Key::kSpace) {
				m_RbCube->SetOrigin(0, 10, 0);
				m_RbCube->SetRotation({ 1, 1, 1 }, glm::radians(45.F));
			}

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

	m_Rdr->SetCamera(m_Cam);
	m_Rdr->SetClearColor(.1F, .1F, .1F, 1.F);

	auto sceneLoader = IO::Loader::Scene();
	if (sceneLoader.Load("assets/3d/scenes/default.obj")) {
		const auto whiteMat = GFX::Material{
			{ 1.F, 1.F, 1.F },
			{ .5F, .5F, .5F },
			32.F
		};

		for (const auto& mesh : sceneLoader.Meshes()) {
			m_Objects.emplace_back(GFX::Object{ mesh });
			m_Objects.back().GetProperties().material = whiteMat;
		}

	} else {
		std::cerr << "Failed to load scene" << std::endl;
		return Status::Fail;
	}

	return Status::Success;
}

auto MyApp::OnUpdate(F64 /*deltaTime*/) -> void
{
	m_Rdr->Clear();

	const GFX::Light lights[] = {
		{
			.position           = { -5.F, 1.F, 5.F },
			.diffuse            = {  3.F, 0.F, 0.F },
			.ambientCoefficient = .005F,
			.attenuation        = .5F,
		},
		{
			.position           = { 5.F, 1.F, 5.F },
			.diffuse            = { 0.F, 3.F, 0.F },
			.ambientCoefficient = .005F,
			.attenuation        = .5F,
		},
		{
			.position           = { 5.F, 1.F, -5.F },
			.diffuse            = { 0.F, 0.F,  3.F },
			.ambientCoefficient = .005F,
			.attenuation        = .5F,
		},
		{
			.position           = { -5.F, 1.F, -5.F },
			.diffuse            = {  3.F, 3.F,  3.F },
			.ambientCoefficient = .005F,
			.attenuation        = .5F,
		}
	};

	for (const auto& object : m_Objects) {
		m_Rdr->SubmitObject(object, lights, std::size(lights), GFX::Renderer::PrimitiveMode::Triangles);
	}

	m_Rdr->Render();
}

auto MyApp::OnFixedUpdate(F64 deltaTime) -> void
{
	m_PhysicsWorld.Update(deltaTime);

	constexpr auto cameraSpeed = 2.0F;
	m_Cam->Move(m_Cam->Front()  * m_CameraDir.z * cameraSpeed * F32(deltaTime));
	m_Cam->Move(m_Cam->Up()     * m_CameraDir.y * cameraSpeed * F32(deltaTime));
	m_Cam->Move(m_Cam->Right()  * m_CameraDir.x * cameraSpeed * F32(deltaTime));
}

auto MyApp::OnShutdown() -> Status
{
	return Status::Success;
}

auto MyApp::OnPacketReceived(U32 sender, Net::Packet packet) -> void
{
	printf("Packet received from %u: %s\n", sender, static_cast<const char*>(packet.Data()));
}

GAZE_REGISTER_APP(MyApp);

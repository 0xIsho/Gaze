#include "Client/App.hpp"

#include "Core/Type.hpp"

#include "WM/Core.hpp"
#include "WM/Window.hpp"

#include "GFX/Renderer.hpp"

#include <glm/gtc/matrix_transform.hpp>

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
	m_Rdr->SetClearColor(.7F, .7F, .7F, 1.F);
	m_Rdr->Clear();
	m_Rdr->SetClearColor(0.F, 0.F, 0.F, 1.F);

	m_Win->OnClose([this] {
		Quit();
	});
}

auto MyApp::OnInit() -> Status
{
	m_Win->Show();

	return Status::Success;
}

auto MyApp::OnUpdate() -> void
{
	auto render = [this] {
		const auto model = glm::rotate(glm::mat4(1.F), glm::radians(30.F), { 0.F, 0.F, 1.F });
		// const auto model = glm::mat4(1.0F);
		const auto view = glm::mat4(1.0F);
		const auto projection = glm::ortho(-1.F, 1.F, -1.F, 1.F);
		const auto mvp = projection * view * model;

		m_Rdr->DrawLine(glm::vec4{ -1, 0, 0, 1 } * mvp, glm::vec4{ 1, 0, 0, 1 } * mvp);
		m_Rdr->DrawLine(glm::vec4{ 0, -1, 0, 1 } * mvp, glm::vec4{ 0, 1, 0, 1 } * mvp);

		for (auto i = -1.F; i <= 1.F; i += .1F) {
			m_Rdr->DrawLine(glm::vec4{ i, -.0075F, 0, 1 } * mvp, glm::vec4{ i, .0075F, 0, 1 } * mvp);
			m_Rdr->DrawLine(glm::vec4{ -.0075F, i, 0, 1 } * mvp, glm::vec4{ .0075F, i, 0, 1 } * mvp);

			for (auto j = -1.F; j <= 1.F; j += .1F) {
				m_Rdr->DrawPoint(glm::vec4{ i, j, 0.0F, 1.0F } * mvp);
			}
		}

		m_Rdr->DrawTri({{ glm::vec4{ -.6F, .4F , 0, 1 } * mvp,glm::vec4 { -.5F, .6F, 0, 1 } * mvp, glm::vec4{ -.4F, .4F, 0, 1 } * mvp }});
		m_Rdr->FillTri({{ glm::vec4{ -.6F, -.4F , 0, 1 } * mvp,glm::vec4 { -.5F, -.6F, 0, 1 } * mvp, glm::vec4{ -.4F, -.4F, 0, 1 } * mvp }});

		m_Rdr->DrawTri({{ glm::vec4{ -.1F, .1F, 0, 1 } * mvp, glm::vec4{ -.1F, -.1F, 0, 1 } * mvp, glm::vec4{ .1F, -.1F, 0, 1 } * mvp }});
		m_Rdr->FillTri({{ glm::vec4{ .1F, -.1F, 0, 1 } * mvp, glm::vec4{ .1F, .1F, 0, 1 } * mvp, glm::vec4{ -.1F, .1F, 0, 1 } * mvp }});

		m_Rdr->DrawTri({{ glm::vec4{ .6F, -.4F, 0, 1 } * mvp, glm::vec4{ .4F, -.4F, 0, 1 } * mvp, glm::vec4{ .5F, -.6F, 0, 1 } * mvp }});
		m_Rdr->FillTri({{ glm::vec4{ .6F, .4F, 0, 1 } * mvp, glm::vec4{ .5F, .6F, 0, 1 } * mvp, glm::vec4{ .4F, .4F, 0, 1 } * mvp }});
	};

	for (int i = 0; i < 2; i++) {
		for(int j = 0; j < 2; j++) {
			m_Rdr->SetViewport(
				(m_Win->Width() / 2) * i + 5,
				(m_Win->Height() / 2) * j + 5,
				m_Win->Width() / 2 - 10,
				m_Win->Height() / 2 - 10
			);
			m_Rdr->Clear();
			render();
		}
	}

	m_Rdr->Render();
}

auto MyApp::OnShutdown() -> Status
{
	return Status::Success;
}

GAZE_REGISTER_APP(MyApp);

#include "Client/App.hpp"

#include <cstdio>

class MyApp : public Gaze::Client::App
{
public:
	MyApp(int argc, char** argv);

private:
		auto OnInit() -> Status override;
		auto OnUpdate() -> void override;
		auto OnShutdown() -> Status override;
};

MyApp::MyApp(int argc, char** argv)
	: App(argc, argv)
{
}

auto MyApp::OnInit() -> Status
{
	puts("Hello, world!");

	return App::Status::Success;
}

auto MyApp::OnUpdate() -> void
{
	puts("Updating...");

	Quit();
}

auto MyApp::OnShutdown() -> Status
{
	puts("Goodbye, world!");

	return App::Status::Success;
}

GAZE_REGISTER_APP(MyApp);

#include "Client/App.hpp"

#include <cstdio>

using namespace Gaze;

class MyApp : public Client::App
{
public:
	MyApp(int argc, char** argv);

private:
	auto OnInit() -> Status override;
	auto OnUpdate(F64 deltaTime) -> void override;
	auto OnShutdown() -> Status override;
};

MyApp::MyApp(int argc, char** argv)
	: App(argc, argv)
{
}

auto MyApp::OnInit() -> Status
{
	puts("Hello, world!");

	return Status::Success;
}

auto MyApp::OnUpdate(F64 deltaTime) -> void
{
	puts("Updating...");

	Quit();
}

auto MyApp::OnShutdown() -> Status
{
	puts("Goodbye, world!");

	return Status::Success;
}

GAZE_REGISTER_APP(MyApp);

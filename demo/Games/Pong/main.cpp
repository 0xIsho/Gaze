#include "Client/App.hpp"

#include "Core/Type.hpp"

using namespace Gaze;

class MyApp : public Client::App
{
public:
	MyApp(int argc, char** argv);

private:
	auto OnInit() -> Status;
	auto OnUpdate(F64 deltaTime) -> void;
	auto OnShutdown() -> Status;
};

MyApp::MyApp(int argc, char** argv)
	: App(argc, argv)
{
}

auto MyApp::OnInit() -> Status
{
	return Status::Success;
}

auto MyApp::OnUpdate(F64 deltaTime) -> void
{

}

auto MyApp::OnShutdown() -> Status
{
	return Status::Success;
}

GAZE_REGISTER_APP(MyApp);

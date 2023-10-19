#include "Client/App.hpp"

#include <iostream>

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
	std::cout << "Initializing MyApp\n";
	return Status::Success;
}

auto MyApp::OnUpdate() -> void
{
	std::cout << "Updating MyApp\n";
}

auto MyApp::OnShutdown() -> Status
{
	std::cout << "Shutting down MyApp\n";
	return Status::Success;
}

auto main(int argc, char** argv) -> int
{
	auto app = MyApp(argc, argv);
	return app.Run() == Gaze::Client::App::Status::Success ? 0 : 1;
}

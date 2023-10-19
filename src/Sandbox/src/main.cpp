#include "Client/App.hpp"

class MyApp : public Gaze::Client::App
{
public:
	MyApp(int argc, char** argv);

private:
};

MyApp::MyApp(int argc, char** argv)
	: App(argc, argv)
{
}

auto main(int argc, char** argv) -> int
{
	auto app = MyApp(argc, argv);
	return app.Run() == Gaze::Client::App::Status::Success ? 0 : 1;
}

#include "Client/App.hpp"

using namespace Gaze;

class MyApp : public Client::ServerApp
{
public:
	MyApp(int argc, char** argv);

private:
	auto OnInit() -> Status;
	auto OnUpdate(F64 deltaTime) -> void;
	auto OnShutdown() -> Status;
	auto OnPacketReceived(U32 sender, Net::Packet packet) -> void override;
};

MyApp::MyApp(int argc, char** argv)
	: ServerApp(argc, argv)
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

auto MyApp::OnPacketReceived(U32 sender, Net::Packet packet) -> void
{
	printf("Packet received from %u: %s\n", sender, static_cast<const char*>(packet.Data()));
}

GAZE_REGISTER_APP(MyApp);

#include "Client/App.hpp"

using namespace Gaze;

class BrickPopperServer : public Client::ServerApp
{
public:
	BrickPopperServer(int argc, char** argv);

private:
	auto OnInit() -> Status override;
	auto OnUpdate(F64 deltaTime) -> void override;
	auto OnFixedUpdate(F64 deltaTime) -> void override;
	auto OnShutdown() -> Status override;
	auto OnPacketReceived(U32 /*sender*/, Net::Packet /*packet*/) -> void override;
	auto OnClientConnected(U32 clientID) -> void override;
	auto OnClientDisconnected(U32 clientID) -> void override;
};

BrickPopperServer::BrickPopperServer(int argc, char** argv)
	: ServerApp(argc, argv)
{
}

auto BrickPopperServer::OnInit() -> Status
{
	return Status::Success;
}

auto BrickPopperServer::OnUpdate(F64 deltaTime) -> void
{

}

auto BrickPopperServer::OnFixedUpdate(F64 deltaTime) -> void
{

}

auto BrickPopperServer::OnShutdown() -> Status
{
	return Status::Success;
}

auto BrickPopperServer::OnPacketReceived(U32 /*sender*/, Net::Packet /*packet*/) -> void
{

}

auto BrickPopperServer::OnClientConnected(U32 clientID) -> void
{

}

auto BrickPopperServer::OnClientDisconnected(U32 clientID) -> void
{

}

GAZE_REGISTER_APP(BrickPopperServer);

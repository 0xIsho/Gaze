#pragma once

#include "Core/Type.hpp"

namespace Gaze::Net {
	class Server
	{
		struct Impl;

	public:
		Server(U32 host = 0, U16 port = 54321);
		~Server();

		auto Update() -> void;

	private:
		Impl* m_pImpl;
	};
}

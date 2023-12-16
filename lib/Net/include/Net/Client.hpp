#pragma once

#include "Core/Type.hpp"

#include <string_view>

namespace Gaze::Net {
	class Client
	{
		struct Impl;

	public:
		Client();
		~Client();

		[[nodiscard]]
		auto Connect(std::string_view host, U16 port) -> bool;
		auto Disconnect() -> void;

		auto Update() -> void;

	private:
		Impl* m_pImpl;
	};
}

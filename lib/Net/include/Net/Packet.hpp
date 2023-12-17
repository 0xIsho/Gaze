#pragma once

#include "Core/Type.hpp"

namespace Gaze::Net {
	class Packet
	{
		struct Impl;

	public:
		Packet(const void* data, U64 size);
		Packet(const Packet& other);
		Packet(Packet&& other) noexcept;
		~Packet();

		auto operator=(const Packet& other) -> Packet&;
		auto operator=(Packet&& other) noexcept -> Packet&;

		[[nodiscard]]
		auto Handle() const -> void*;
		[[nodiscard]]
		auto Data() const -> void*;

		auto HasOwnership(bool val) -> void;

	private:
		Impl* m_pImpl;
	};
}

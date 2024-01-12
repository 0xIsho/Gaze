#pragma once

#include "Core/Type.hpp"

namespace Gaze::Physics {
	class Rigidbody;

	class World
	{
		struct Impl;

	public:
		World();
		~World();

		auto Update(F64 timestep) -> void;

		auto AddRigidbody(Mem::Shared<Rigidbody> body) -> void;

		[[nodiscard]] auto Handle() const noexcept -> void*;

	private:
		struct Impl* m_pImpl;
	};
}

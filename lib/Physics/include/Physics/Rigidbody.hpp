#pragma once

#include "Core/Type.hpp"

#include "glm/vec3.hpp"

namespace Gaze::Physics {
	class Shape;

	class Rigidbody
	{
		struct Impl;

		static constexpr auto kDefaultMass = 1.0F;

	public:
		Rigidbody(Shared<Shape> collisionShape, float mass = kDefaultMass);
		~Rigidbody();

		[[nodiscard]] auto Handle() const noexcept -> void*;

		              auto SetOrigin(float x, float y, float z)              noexcept -> void;
		[[nodiscard]] auto Origin()                                    const noexcept -> glm::vec3;
		              auto SetRotation(const glm::vec3& axis, float radians) noexcept -> void;
		[[nodiscard]] auto RotationAxis()                              const noexcept -> glm::vec3;
		[[nodiscard]] auto RotationAngle()                             const noexcept -> float;
		              auto Translate(const glm::vec3& translation)           noexcept -> void;

	private:
		Impl* m_pImpl;
	};
}

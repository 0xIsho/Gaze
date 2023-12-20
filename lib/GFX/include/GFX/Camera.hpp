#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace Gaze::GFX {
	class Camera
	{
	public:
		[[nodiscard]]
		auto ComputeViewMatrix() const -> glm::mat4;

		auto Move(glm::vec3 dir) -> void;

		[[nodiscard]]
		auto Position() const -> const glm::vec3& { return m_Position; }
		[[nodiscard]]
		auto Front() const -> const glm::vec3& { return m_Front; }
		[[nodiscard]]
		auto Up() const -> const glm::vec3& { return m_Up; }

	private:
		glm::vec3 m_Position { .0F, .0F, 1.0F };
		glm::vec3 m_Front { .0F, .0F, -1.F };
		glm::vec3 m_Up { .0F, 1.0F, .0F };
	};
}

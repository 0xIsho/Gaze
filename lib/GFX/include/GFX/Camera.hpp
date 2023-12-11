#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace Gaze::GFX {
	class Camera
	{
	public:
		[[nodiscard]]
		auto ComputeViewMatrix() const -> glm::mat4;

	private:
		glm::vec3 m_Position { .0F, 0.0F, 1.0F };
		glm::vec3 m_Front { .0F, .0F, -1.F };
		glm::vec3 m_Up { .0F, 1.0F, .0F };
	};
}

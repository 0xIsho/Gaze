#include "GFX/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Gaze::GFX {
	auto Camera::ComputeViewMatrix() const -> glm::mat4
	{
		return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	auto Camera::Move(glm::vec3 dir) -> void
	{
		m_Position += dir;
	}
}

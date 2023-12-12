#include "GFX/Mesh.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <utility>

namespace Gaze::GFX {
	Mesh::Mesh(std::initializer_list<glm::vec3> vertices, std::initializer_list<I64> indices)
		: m_Vertices(std::move(vertices))
		, m_Indices(std::move(indices))
		, m_Transform(1.0F)
	{
	}

	auto Mesh::SetPosition(const glm::vec3& position) -> void
	{
		m_Transform[3] = glm::vec4(position, 1.0F);
	}

	auto Mesh::Translate(const glm::vec3& translation) -> void
	{
		m_Transform = glm::translate(m_Transform, translation);
	}

	auto Mesh::Rotate(F32 radians, const glm::vec3& axis) -> void
	{
		m_Transform *= glm::mat4_cast(glm::angleAxis(radians, axis));
	}

	auto Mesh::Scale(const glm::vec3& scale) -> void
	{
		m_Transform = glm::scale(m_Transform, scale);
	}
}

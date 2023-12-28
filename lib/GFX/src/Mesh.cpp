#include "GFX/Mesh.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <utility>

namespace Gaze::GFX {
	Mesh::Mesh(std::initializer_list<Vertex> vertices, std::initializer_list<U32> indices)
		: m_Vertices(std::move(vertices))
		, m_Indices(std::move(indices))
		, m_Transform(1.0F)
		, m_Material({ { 1.F, 1.F, 1.F, 1.F } })
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
		m_Transform[0][0] = scale[0];
		m_Transform[1][1] = scale[1];
		m_Transform[2][2] = scale[2];
	}
}

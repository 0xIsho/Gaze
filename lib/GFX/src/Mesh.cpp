#include "GFX/Mesh.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <utility>

namespace Gaze::GFX {
	Mesh::Mesh(std::initializer_list<Vertex> vertices, std::initializer_list<U32> indices)
		: Mesh({ Primitive{ std::move(vertices), std::move(indices) } })
	{
	}

	Mesh::Mesh(std::initializer_list<Primitive> primitives)
		: m_Primitives(std::move(primitives))
		, m_Props(glm::mat4{ 1.0F }, { { 1.F, 1.F, 1.F }, { .5F, .5F, .5F }, 32.F })
	{
	}

	auto Mesh::SetPosition(const glm::vec3& position) noexcept -> void
	{
		m_Props.transform[3] = glm::vec4(position, 1.0F);
	}

	auto Mesh::Translate(const glm::vec3& translation) noexcept -> void
	{
		m_Props.transform = glm::translate(m_Props.transform, translation);
	}

	auto Mesh::Rotate(F32 radians, const glm::vec3& axis) noexcept -> void
	{
		m_Props.transform *= glm::mat4_cast(glm::angleAxis(radians, axis));
	}

	auto Mesh::Scale(const glm::vec3& scale) noexcept -> void
	{
		m_Props.transform[0][0] = scale[0];
		m_Props.transform[1][1] = scale[1];
		m_Props.transform[2][2] = scale[2];
	}
}

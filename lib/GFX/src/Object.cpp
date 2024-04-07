#include "GFX/Object.hpp"

namespace Gaze::GFX {
	Object::Object(Geometry::Mesh mesh)
		: Object(std::move(mesh), Properties({ .transform = glm::mat4(1.0F), .material = {} }))
	{
	}

	Object::Object(Geometry::Mesh mesh, struct Properties props)
		: m_Mesh(std::move(mesh))
		, m_Properties(std::move(props))
	{
	}

	auto Object::SetPosition(const glm::vec3& pos) -> void
	{
		m_Properties.transform[3] = { pos, 1.0F };

	}

	auto Object::Rotate(float angleRadians, const glm::vec3& axis) -> void
	{
		m_Properties.transform *= glm::mat4_cast(glm::angleAxis(glm::radians(-90.0F), glm::vec3{ 1.0F, .0F, .0F }));
	}
}

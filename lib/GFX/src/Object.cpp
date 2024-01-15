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
}

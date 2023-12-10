#include "GFX/Mesh.hpp"

#include <utility>

namespace Gaze::GFX {
	Mesh::Mesh(std::initializer_list<glm::vec3> vertices, std::initializer_list<I64> indices)
		: m_Vertices(std::move(vertices))
		, m_Indices(std::move(indices))
	{
	}
}

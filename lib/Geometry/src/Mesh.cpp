#include "Geometry/Mesh.hpp"

#include <utility>

namespace Gaze::Geometry {
	Mesh::Mesh(std::initializer_list<Vertex> vertices, std::initializer_list<Index> indices)
		: Mesh({ Primitive{ std::move(vertices), std::move(indices) } })
	{
	}

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<Index> indices)
		: Mesh({ Primitive{ std::move(vertices), std::move(indices) } })
	{
	}

	Mesh::Mesh(std::initializer_list<Primitive> primitives)
		: Mesh(std::vector<Primitive>(std::move(primitives)))
	{
	}

	Mesh::Mesh(std::vector<Primitive> primitives)
		: m_Primitives{ std::move(primitives) }
	{
	}
}

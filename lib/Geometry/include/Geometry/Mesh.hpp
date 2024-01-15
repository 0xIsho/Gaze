#pragma once

#include "Core/Type.hpp"

#include <vector>
#include <initializer_list>

namespace Gaze::Geometry {
	struct Vertex
	{
		float x, y, z;    /**< Coordinates */
		float nx, ny, nz; /**< Normals */
	};

	using Index = U32;

	struct Primitive
	{
		std::vector<Vertex> vertices;
		std::vector<Index>  indices;
	};

	class Mesh
	{
	public:
		static constexpr auto kVertexSize = sizeof(Vertex);
		static constexpr auto kIndexSize  = sizeof(Index);

	public:
		Mesh(std::initializer_list<Vertex> vertices, std::initializer_list<Index> indices);
		Mesh(std::vector<Vertex> vertices, std::vector<Index> indices);
		Mesh(std::initializer_list<Primitive> primitives);
		Mesh(std::vector<Primitive> primitives);

		[[nodiscard]] auto Primitives() const noexcept -> const std::vector<Primitive>&;

	private:
		std::vector<Primitive> m_Primitives;
	};

	inline auto Mesh::Primitives() const noexcept -> const std::vector<Primitive>&
	{
		return m_Primitives;
	}
}

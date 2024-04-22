#pragma once

#include "Core/Type.hpp"

#include <vector>
#include <initializer_list>

namespace Gaze::Geometry {
	/**
	 * @brief Represents a vertex in a mesh.
	 */
	struct Vertex
	{
		float x, y, z;    /**< Coordinates */
		float nx, ny, nz; /**< Normals */
	};

	/**
	 * @brief Represents an index in a mesh.
	 */
	using Index = U32;

	/**
	 * @brief Represents a primitive in a mesh.
	 */
	struct Primitive
	{
		std::vector<Vertex> vertices; /**< List of vertices in the primitive */
		std::vector<Index>  indices;  /**< List of indices in the primitive */
	};

	/**
	 * @brief Represents a geometric mesh.
	 */
	class Mesh
	{
	public:
		/**
		 * @brief Size of a vertex in bytes.
		 */
		static constexpr auto kVertexSize = sizeof(Vertex);
		/**
		 * @brief Size of an index in bytes.
		 */
		static constexpr auto kIndexSize  = sizeof(Index);

	public:
		/**
		 * @brief Constructs a mesh from a list of primitives.
		 *
		 * @param primitives List of primitives.
		 */
		Mesh(std::initializer_list<Primitive> primitives);
		/**
		 * @brief Constructs a mesh from a list of primitives.
		 *
		 * @param primitives List of primitives.
		 */
		Mesh(std::vector<Primitive> primitives);
		/**
		 * @brief Constructs a mesh from lists of vertices and indices.
		 *
		 * @param vertices List of vertices.
		 * @param indices List of indices.
		 */
		Mesh(std::vector<Vertex> vertices, std::vector<Index> indices);
		/**
		 * @brief Constructs a mesh from lists of vertices and indices.
		 *
		 * @param vertices List of vertices.
		 * @param indices List of indices.
		 */
		Mesh(std::initializer_list<Vertex> vertices, std::initializer_list<Index> indices);

		/**
		 * @brief Returns the list of primitives in the mesh.
		 */
		[[nodiscard]] auto Primitives() const noexcept -> const std::vector<Primitive>&;

	private:
		std::vector<Primitive> m_Primitives; /**< List of primitives in the mesh */
	};

	inline auto Mesh::Primitives() const noexcept -> const std::vector<Primitive>&
	{
		return m_Primitives;
	}
}

#pragma once

#include "Core/Type.hpp"

#include <glm/vec3.hpp>

#include <vector>
#include <initializer_list>

namespace Gaze::GFX {
	class Mesh
	{
	public:
		Mesh(std::initializer_list<glm::vec3> vertices, std::initializer_list<I64> indices);

		[[nodiscard]]
		auto Vertices() const -> const std::vector<glm::vec3>&;
		[[nodiscard]]
		auto Indices() const -> const std::vector<I64>&;

	private:
		std::vector<glm::vec3> m_Vertices;
		std::vector<I64> m_Indices;
	};

	inline auto Mesh::Vertices() const -> const std::vector<glm::vec3>&
	{
		return m_Vertices;
	}

	inline auto Mesh::Indices() const -> const std::vector<I64>&
	{
		return m_Indices;
	}
}

#pragma once

#include "Core/Type.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

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
		[[nodiscard]]
		auto Transform() const -> const glm::mat4&;

		auto Translate(const glm::vec3& translation) -> void;
		auto Rotate(F32 radians, const glm::vec3& axis) -> void;
		auto Scale(const glm::vec3& scale) -> void;

	private:
		std::vector<glm::vec3> m_Vertices;
		std::vector<I64> m_Indices;
		glm::mat4 m_Transform;
	};

	inline auto Mesh::Vertices() const -> const std::vector<glm::vec3>&
	{
		return m_Vertices;
	}

	inline auto Mesh::Indices() const -> const std::vector<I64>&
	{
		return m_Indices;
	}

	inline auto Mesh::Transform() const -> const glm::mat4&
	{
		return m_Transform;
	}
}

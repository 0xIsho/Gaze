#pragma once

#include "Core/Type.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <vector>
#include <initializer_list>

namespace Gaze::GFX {
	struct Vertex
	{
		glm::vec3 position;
	};

	class Mesh
	{
	public:
		Mesh(std::initializer_list<Vertex> vertices, std::initializer_list<U32> indices);

		[[nodiscard]]
		auto Vertices() const -> const std::vector<Vertex>&;
		[[nodiscard]]
		auto Indices() const -> const std::vector<U32>&;
		[[nodiscard]]
		auto Transform() const -> const glm::mat4&;

		auto SetPosition(const glm::vec3& position) -> void;

		auto Translate(const glm::vec3& translation) -> void;
		auto Rotate(F32 radians, const glm::vec3& axis) -> void;
		auto Scale(const glm::vec3& scale) -> void;

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<U32> m_Indices;
		glm::mat4 m_Transform;
	};

	inline auto Mesh::Vertices() const -> const std::vector<Vertex>&
	{
		return m_Vertices;
	}

	inline auto Mesh::Indices() const -> const std::vector<U32>&
	{
		return m_Indices;
	}

	inline auto Mesh::Transform() const -> const glm::mat4&
	{
		return m_Transform;
	}
}

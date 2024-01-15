#pragma once

#include "Core/Type.hpp"

#include "GFX/Material.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <vector>
#include <initializer_list>

namespace Gaze::GFX {
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normals;
	};

	struct Primitive
	{
		std::vector<Vertex> vertices;
		std::vector<U32>    indices;
	};

	class Mesh
	{
	public:
		static constexpr auto kVertexSize = I32(sizeof(Vertex));
		static constexpr auto kIndexSize  = I32(sizeof(U32));

		struct Properties
		{
			glm::mat4       transform;
			struct Material material;
		};

	public:
		[[deprecated("Use GFX::Object or Geometry::Mesh")]]
		Mesh(std::initializer_list<Vertex> vertices, std::initializer_list<U32> indices);
		[[deprecated("Use GFX::Object or Geometry::Mesh")]]
		Mesh(std::vector<Vertex> vertices, std::vector<U32> indices);
		[[deprecated("Use GFX::Object or Geometry::Mesh")]]
		Mesh(std::initializer_list<Primitive> primitives);

		[[nodiscard]] auto Primitives()           const noexcept -> const std::vector<Primitive>&;
		[[nodiscard]] auto Transform()            const noexcept -> const glm::mat4&;

		auto SetPosition(const glm::vec3& position)     noexcept -> void;
		auto Translate(const glm::vec3& translation)    noexcept -> void;
		auto Rotate(F32 radians, const glm::vec3& axis) noexcept -> void;
		auto Scale(const glm::vec3& scale)              noexcept -> void;

		[[nodiscard]] auto Material()                   noexcept -> struct Material&;
		[[nodiscard]] auto Material()             const noexcept -> const struct Material&;
		auto SetMaterial(struct Material mat)           noexcept -> void;

	private:
		std::vector<Primitive> m_Primitives;
		Properties             m_Props;
	};

	inline auto Mesh::Primitives() const noexcept -> const std::vector<Primitive>&
	{
		return m_Primitives;
	}

	inline auto Mesh::Transform() const noexcept -> const glm::mat4&
	{
		return m_Props.transform;
	}

	inline auto Mesh::Material() noexcept -> struct Material&
	{
		return m_Props.material;
	}

	inline auto Mesh::Material() const noexcept -> const struct Material&
	{
		return m_Props.material;
	}

	inline auto Mesh::SetMaterial(struct Material mat) noexcept -> void
	{
		m_Props.material = std::move(mat);
	}
}

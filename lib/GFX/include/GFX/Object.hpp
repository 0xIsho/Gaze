#pragma once

#include "GFX/Material.hpp"

#include "Geometry/Mesh.hpp"

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace Gaze::GFX {
	class Object
	{
	public:
		struct Properties
		{
			glm::mat4 transform;
			Material  material;
		};

	public:
		Object(Geometry::Mesh mesh);
		Object(Geometry::Mesh mesh, Properties props);

		[[nodiscard]] auto Mesh()          const noexcept -> const Geometry::Mesh&;

		auto SetPosition(const glm::vec3& pos) -> void;
		auto Rotate(float angleRadians, const glm::vec3& axis) -> void;

		[[nodiscard]] auto GetProperties() const noexcept -> const Properties&;
		[[nodiscard]] auto GetProperties()       noexcept -> Properties&;

	private:
		Geometry::Mesh m_Mesh;
		Properties     m_Properties;
	};

	inline auto Object::Mesh() const noexcept -> const Geometry::Mesh&
	{
		return m_Mesh;
	}

	inline auto Object::GetProperties() const noexcept -> const Properties&
	{
		return m_Properties;
	}

	inline auto Object::GetProperties() noexcept -> Properties&
	{
		return m_Properties;
	}
}

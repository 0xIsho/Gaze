#pragma once

#include "GFX/Material.hpp"

#include "Geometry/Mesh.hpp"

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace Gaze::GFX {
	/**
	 * @brief Represents a renderable object.
	 */
	class Object
	{
	public:
		/**
		 * @brief Object properties.
		 */
		struct Properties
		{
			glm::mat4 transform;
			Material  material;
		};

	public:
		/**
		 * @brief Construct a new Object.
		 *
		 * @param mesh The mesh to render.
		 */
		Object(Geometry::Mesh mesh);

		/**
		 * @brief Construct a new Object.
		 *
		 * @param mesh The mesh to render.
		 * @param props The object properties.
		 */
		Object(Geometry::Mesh mesh, Properties props);

		/**
		 * @brief Get the mesh associated with the object.
		 *
		 * @return The mesh.
		 */
		[[nodiscard]] auto Mesh() const noexcept -> const Geometry::Mesh&;

		/**
		 * @brief Set the object's position.
		 *
		 * @param pos The position.
		 */
		auto SetPosition(const glm::vec3& pos) -> void;
		/**
		 * @brief Rotate the object.
		 *
		 * @param angleRadians The rotation angle in radians.
		 * @param axis The rotation axis.
		 */
		auto Rotate(float angleRadians, const glm::vec3& axis) -> void;

		/**
		 * @brief Get the object's properties.
		 *
		 * @return The object's properties.
		 */
		[[nodiscard]] auto GetProperties() const noexcept -> const Properties&;
		/**
		 * @brief Get the object's properties.
		 *
		 * @return The object's properties.
		 */
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

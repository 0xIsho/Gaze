#pragma once

#include "Core/Type.hpp"

#include "GFX/Mesh.hpp"
#include "GFX/Object.hpp"

#include <glm/vec3.hpp>

#include <array>

namespace Gaze::GFX {
	[[deprecated("Use GFX::Primitives::CreatePoint()")]]
	[[nodiscard]] auto CreatePoint(glm::vec3 point)                          -> Mesh;
	[[deprecated("Use GFX::Primitives::CreateLine()")]]
	[[nodiscard]] auto CreateLine(glm::vec3 start, glm::vec3 end)            -> Mesh;
	[[deprecated("Use GFX::Primitives::CreateTriangle()")]]
	[[nodiscard]] auto CreateTriangle(std::array<glm::vec3, 3> points)       -> Mesh;
	[[deprecated("Use GFX::Primitives::CreateQuad()")]]
	[[nodiscard]] auto CreateQuad(glm::vec3 position, F32 width, F32 height) -> Mesh;

	namespace Primitives {
		/**
		 * @brief Create an Object representing a point
		 *
		 * @param point The position of the point
		 *
		 * @return The created Object
		 */
		[[nodiscard]] auto CreatePoint(glm::vec3 point) -> Object;
		/**
		 * @brief Create an Object representing a line
		 *
		 * @param start The start of the line
		 * @param end The end of the line
		 *
		 * @return The created Object
		 */
		[[nodiscard]] auto CreateLine(glm::vec3 start, glm::vec3 end) -> Object;
		/**
		 * @brief Create an Object representing a triangle
		 *
		 * @param points The points of the triangle
		 *
		 * @return The created Object
		 */
		[[nodiscard]] auto CreateTriangle(std::array<glm::vec3, 3> points) -> Object;
		/**
		 * @brief Create an Object representing a quad
		 *
		 * @param position The position of the quad
		 * @param width The width of the quad
		 * @param height The height of the quad
		 *
		 * @return The created Object
		 */
		[[nodiscard]] auto CreateQuad(glm::vec3 position, F32 width, F32 height) -> Object;
	}
}

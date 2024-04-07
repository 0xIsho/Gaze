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
		[[nodiscard]] auto CreatePoint(glm::vec3 point) -> Object;
		[[nodiscard]] auto CreateLine(glm::vec3 start, glm::vec3 end) -> Object;
		[[nodiscard]] auto CreateTriangle(std::array<glm::vec3, 3> points) -> Object;
		[[nodiscard]] auto CreateQuad(glm::vec3 position, F32 width, F32 height) -> Object;
	}
}

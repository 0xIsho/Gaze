#pragma once

#include "Core/Type.hpp"

#include "GFX/Mesh.hpp"

#include <glm/vec3.hpp>

#include <array>

namespace Gaze::GFX {
	[[nodiscard]]
	auto CreatePoint(glm::vec3 point) -> Mesh;
	[[nodiscard]]
	auto CreateLine(glm::vec3 start, glm::vec3 end) -> Mesh;
	[[nodiscard]]
	auto CreateTriangle(std::array<glm::vec3, 3> points) -> Mesh;
	[[nodiscard]]
	auto CreateQuad(glm::vec3 position, F32 width, F32 height) -> Mesh;
}

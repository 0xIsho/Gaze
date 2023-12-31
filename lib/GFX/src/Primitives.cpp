#include "GFX/Primitives.hpp"
#include <glm/geometric.hpp>

namespace Gaze::GFX {
	auto CreatePoint(glm::vec3 point) -> Mesh
	{
		return Mesh({ Vertex{ .position = std::move(point), .normals = { .0F, .0F, .0F } } }, { 0 });
	}

	auto CreateLine(glm::vec3 start, glm::vec3 end) -> Mesh
	{
		return Mesh({
			{ .position = std::move(start), .normals = { .0F, .0F, .0F }},
			{ .position = std::move(end)  , .normals = { .0F, .0F, .0F }}
		}, { 0, 1 });
	}

	auto CreateTriangle(std::array<glm::vec3, 3> points) -> Mesh
	{
		const auto normals = glm::normalize(glm::cross(points[1] - points[0], points[2] - points[0]));
		return Mesh({
			{ .position = std::move(points[0]), .normals = normals },
			{ .position = std::move(points[1]), .normals = normals },
			{ .position = std::move(points[2]), .normals = normals }
		}, { 0, 1, 2 });
	}

	auto CreateQuad(glm::vec3 position, F32 width, F32 height) -> Mesh
	{
		const auto v1 = position - glm::vec3(width / 2, .0F, height / 2);
		const auto v2 = v1 + glm::vec3(.0F, .0F, height);
		const auto v3 = v1 + glm::vec3(width, .0F, height);
		const auto v4 = v1 + glm::vec3(width, .0F, .0F);

		const auto normals = glm::normalize(glm::cross(v2 - v1, v3 - v1));

		return Mesh({
			{ .position = v1, .normals = normals },
			{ .position = v2, .normals = normals },
			{ .position = v3, .normals = normals },
			{ .position = v4, .normals = normals },
		}, { 0, 1, 2, 2, 3, 0 });
	}
}

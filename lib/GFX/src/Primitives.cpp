#include "GFX/Primitives.hpp"
#include "Geometry/Mesh.hpp"

#include <glm/geometric.hpp>

namespace Gaze::GFX {
	namespace Primitives {
		auto CreatePoint(glm::vec3 point) -> Object
		{
			return Object(Geometry::Mesh{ { Geometry::Vertex{ point.x, point.y, point.z, .0F, .0F, .0F }}, { Geometry::Index(0) } });
		}

		auto CreateLine(glm::vec3 start, glm::vec3 end) -> Object
		{
			return Object(
				Geometry::Mesh{
					{
						Geometry::Vertex{ start.x, start.y, start.z, .0F, .0F, .0F },
						Geometry::Vertex{ end.x, end.y, end.z, .0F, .0F, .0F }
					},
					{
						Geometry::Index(0),
						Geometry::Index(1)
					}
				});
		}

		auto CreateTriangle(std::array<glm::vec3, 3> points) -> Object
		{
			const auto normals = glm::normalize(glm::cross(points[1] - points[0], points[2] - points[0]));

			return Object(
				Geometry::Mesh{
					{
						Geometry::Vertex{ points[0].x, points[0].y, points[0].z, normals.x, normals.y, normals.z },
						Geometry::Vertex{ points[1].x, points[1].y, points[1].z, normals.x, normals.y, normals.z },
						Geometry::Vertex{ points[2].x, points[2].y, points[2].z, normals.x, normals.y, normals.z },
					},
					{
						Geometry::Index(0),
						Geometry::Index(1),
						Geometry::Index(2)
					}
				});
		}

		auto CreateQuad(glm::vec3 position, F32 width, F32 height) -> Object
		{
			const auto v1 = position - glm::vec3(width / 2, .0F, height / 2);
			const auto v2 = v1 + glm::vec3(.0F, .0F, height);
			const auto v3 = v1 + glm::vec3(width, .0F, height);
			const auto v4 = v1 + glm::vec3(width, .0F, .0F);

			const auto normals = glm::normalize(glm::cross(v2 - v1, v3 - v1));

			return Object(
				Geometry::Mesh{
					{
						Geometry::Vertex{ v1.x, v1.y, v1.z, normals.x, normals.y, normals.z },
						Geometry::Vertex{ v2.x, v2.y, v2.z, normals.x, normals.y, normals.z },
						Geometry::Vertex{ v3.x, v3.y, v3.z, normals.x, normals.y, normals.z },
						Geometry::Vertex{ v4.x, v4.y, v4.z, normals.x, normals.y, normals.z },
					},
					{
						Geometry::Index(0),
						Geometry::Index(1),
						Geometry::Index(2),
						Geometry::Index(2),
						Geometry::Index(3),
						Geometry::Index(0),
					}
				});
		}
	}

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

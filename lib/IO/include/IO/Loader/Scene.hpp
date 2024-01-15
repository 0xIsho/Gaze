#pragma once

#include <vector>
#include <filesystem>

namespace Gaze::IO::Loader {
	struct Vertex
	{
		float x;
		float y;
		float z;

		float nx;
		float ny;
		float nz;
	};

	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
	};

	class Scene
	{
	public:

		[[nodiscard]] auto Load(const std::filesystem::path& path) -> bool;

		[[nodiscard]] auto Meshes() const noexcept -> const std::vector<Mesh>&;

	private:
		std::vector<Mesh> m_Meshes;
	};

	inline auto Scene::Meshes() const noexcept -> const std::vector<Mesh>&
	{
		return m_Meshes;
	}
}

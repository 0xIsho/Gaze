#pragma once

#include "Geometry/Mesh.hpp"

#include <vector>
#include <filesystem>

namespace Gaze::IO::Loader {
	class Scene
	{
	public:

		[[nodiscard]] auto Load(const std::filesystem::path& path) -> bool;

		[[nodiscard]] auto Meshes() const noexcept -> const std::vector<Geometry::Mesh>&;

	private:
		std::vector<Geometry::Mesh> m_Meshes;
	};

	inline auto Scene::Meshes() const noexcept -> const std::vector<Geometry::Mesh>&
	{
		return m_Meshes;
	}
}

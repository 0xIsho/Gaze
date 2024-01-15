#include "IO/Loader/Scene.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <algorithm>

#include <iostream>

namespace Gaze::IO::Loader {
	static auto ProcessScene(const aiScene* scene, std::vector<Mesh>& outMeshes) -> bool
	{
		if (!scene->HasMeshes()) {
			return false;
		}

		for (auto i = 0U; i < scene->mNumMeshes; ++i) {
			const auto* mesh = scene->mMeshes[i];

			auto vertices = std::vector<Vertex>();
			for (auto j = 0U; j < mesh->mNumVertices; ++j) {
				auto vertex = Vertex{
					.x = mesh->mVertices[j].x,
					.y = mesh->mVertices[j].y,
					.z = mesh->mVertices[j].z,
					.nx = 0.0f,
					.ny = 0.0f,
					.nz = 0.0f,
				};
				if (mesh->HasNormals()) {
					vertex.nx = mesh->mNormals[j].x;
					vertex.ny = mesh->mNormals[j].y;
					vertex.nz = mesh->mNormals[j].z;
				}

				vertices.push_back(std::move(vertex));
			}

			auto indices = std::vector<unsigned int>();
			for (auto j = 0U; j < mesh->mNumFaces; ++j) {
				const auto face = mesh->mFaces[j];
				std::copy(face.mIndices, face.mIndices + face.mNumIndices, std::back_inserter(indices));
			}

			outMeshes.emplace_back(std::move(vertices), std::move(indices));
		}

		return true;
	}

	auto Scene::Load(const std::filesystem::path& path) -> bool
	{
		auto importer = Assimp::Importer();

		const auto* scene = importer.ReadFile(
			path.string(),
			aiProcess_CalcTangentSpace |
			aiProcess_GenNormals |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_ValidateDataStructure
		);

		if (scene == nullptr) {
			std::cerr << importer.GetErrorString() << std::endl;
			return false;
		}

		return ProcessScene(scene, m_Meshes);
	}
}

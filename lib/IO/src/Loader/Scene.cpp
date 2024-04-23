#include "IO/Loader/Scene.hpp"

#include "Log/Logger.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <algorithm>

#include <iostream>

namespace Gaze::IO::Loader {
	static auto ProcessNode(const aiNode* node, const aiScene* scene, std::vector<Geometry::Mesh>& outMeshes) -> void
	{
		if (node == nullptr) {
			return;
		}

		for (auto i = 0U; i < node->mNumMeshes; ++i) {
			const auto* mesh = scene->mMeshes[node->mMeshes[i]];

			auto vertices = std::vector<Geometry::Vertex>();
			vertices.reserve(mesh->mNumVertices);
			for (auto j = 0U; j < mesh->mNumVertices; ++j) {
				auto vertex = Geometry::Vertex {
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

			auto indices = std::vector<Geometry::Index>();
			indices.reserve(size_t(mesh->mNumFaces) * 3);
			for (auto j = 0U; j < mesh->mNumFaces; ++j) {
				const auto& face = mesh->mFaces[j];
				std::copy(face.mIndices, face.mIndices + face.mNumIndices, std::back_inserter(indices));
			}

			outMeshes.emplace_back(std::move(vertices), std::move(indices));
		}

		for (auto i = 0U; i < node->mNumChildren; ++i) {
			ProcessNode(node->mChildren[i], scene, outMeshes);
		}
	}

	static auto ProcessScene(const aiScene* scene, std::vector<Geometry::Mesh>& outMeshes) -> bool
	{
		if (!scene->HasMeshes()) {
			return false;
		}

		ProcessNode(scene->mRootNode, scene, outMeshes);
		return true;
	}

	auto Scene::Load(const std::filesystem::path& path) -> bool
	{
		auto importer = Assimp::Importer();
		auto logger = Log::Logger("Loader");

		if (!std::filesystem::exists(path)) {
			logger.Error("Scene file '{}' does not exist.", path.string());
			return false;
		}

		logger.Info("Loading scene from: {}", path.string());

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
			logger.Error("Error loading scene: {}", importer.GetErrorString());
			return false;
		}

		return ProcessScene(scene, m_Meshes);
	}
}

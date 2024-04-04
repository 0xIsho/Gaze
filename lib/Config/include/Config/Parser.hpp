#pragma once

#include <string>
#include <filesystem>

namespace Gaze::Config {
	class Section;
	class Configuration;

	class Parser
	{
	public:
		using Path = std::filesystem::path;

	public:
		Parser(Configuration* outConfig);

		// "path" relative to "s_ConfigDirPath"
		[[nodiscard]] auto Load(const Path& path) -> bool;

		static auto ConfigDirectoryPath(const Path& path) -> void;

	private:
		auto ParseSection(const std::string& sectionStr, std::string& outSectionPath) -> bool;
		auto ParseParameter(const std::string& param, Section& outSection) -> bool;

	private:
		Configuration* m_pOutConfig = nullptr;

		static Path s_ConfigDirPath;
	};
}

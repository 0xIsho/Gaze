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
		struct Impl;

	public:
		Parser(Configuration* outConfig);
		~Parser();

		// "path" relative to "s_ConfigDirPath"
		[[nodiscard]] auto Load(const Path& path) -> bool;

		static auto ConfigDirectoryPath(const Path& path) -> void;

	private:
		auto ParseSection(const std::string& sectionStr, std::string& outSectionPath) -> bool;
		auto ParseParameter(const std::string& param, Section& outSection) -> bool;

	private:
		Impl* m_pImpl = nullptr;
	};
}

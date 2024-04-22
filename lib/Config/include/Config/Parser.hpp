#pragma once

#include <string>
#include <filesystem>

namespace Gaze::Config {
	class Section;
	class Configuration;

	/**
	* @brief This class is used to parse a configuration file.
	*
	* The configuration file is expected to be in the following format:
	*
	* [section]
	* parameter = value
	* 
	* "section" and "parameter" are case-sensitive. "section" and "value" are
	* optional and may be omited. The default section, if omited, is '/'.
	*
	* The "section" is optional. If the "section" is not specified, the "parameter" will be added to the "default" section.
	*
	* The "parameter" is optional. If the "parameter" is not specified, the "value" will be ignored.
	*
	* The "value" is optional. If the "value" is not specified, the "parameter" will be set to an empty string.
	*/
	class Parser
	{
	public:
		using Path = std::filesystem::path;
		struct Impl;

	public:
		/**
		 * @brief Construct a Parser object
		 *
		 * @param outConfig Output parameter where parsed configuration will be
		 *   stored. MUST be a valid pointer!
		 */
		Parser(Configuration* outConfig);

		/**
		 * @brief Destroy the parser object
		 */
		~Parser();

		// "path" relative to "s_ConfigDirPath"
		/**
		 * @brief Load/Parse a configuration file
		 *
		 * @param path The path to the configuration file; relative to "ConfigDirectoryPath"
		 *
		 * @return true If the parsing was successful, false otherwise
		 *
		 * @note The output configuration parameter (provided in the constructor) is
		 *   not modified in case of failure.
		 */
		[[nodiscard]] auto Load(const Path& path) -> bool;

		/**
		 * @brief Set the root of the configuration directory
		 *
		 * Configuration file lookups, in @ref Load() for example, are relative to
		 * the path provided here. Defaults to the executables current working directory.
		 */
		static auto ConfigDirectoryPath(const Path& path) -> void;

	private:
		auto ParseSection(const std::string& sectionStr, std::string& outSectionPath) -> bool;
		auto ParseParameter(const std::string& param, Section& outSection) -> bool;

	private:
		Impl* m_pImpl = nullptr;
	};
}

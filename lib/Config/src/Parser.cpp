#include "Config/Parser.hpp"

#include "Config/Configuration.hpp"

#include "Debug/Assert.hpp"

#include "Log/Logger.hpp"

#include <string>
#include <fstream>

namespace Gaze::Config {
	static auto Trim(std::string& str, const std::string& whitespaceChars = " \t") -> void
	{
		auto strBegin = str.find_first_not_of(whitespaceChars);

		if (strBegin == std::string::npos) {
			return;
		}

		auto strEnd = str.find_last_not_of(whitespaceChars);
		auto range = strEnd - strBegin + 1;

		str = str.substr(strBegin, range);
	}

	static auto RemoveComments(std::string& str, const std::string& commentChars = ";") -> void
	{
		const auto commentBegin = str.find_first_of(commentChars);
		if (commentBegin == std::string::npos) {
			return;
		}

		str.erase(str.begin() + commentBegin, str.end());
	}

	struct Parser::Impl
	{
		Configuration* outConfig = nullptr;
		Log::Logger logger = Log::Logger("Parser");

		static Path s_ConfigDirPath;
	};

	Parser::Path Parser::Impl::s_ConfigDirPath = ".";

	auto Parser::ConfigDirectoryPath(const Path& path) -> void
	{
		Impl::s_ConfigDirPath = path;
	}

	Parser::Parser(Configuration* outConfig)
		: m_pImpl(new Impl({ .outConfig = outConfig }))
	{
		GAZE_ASSERT(outConfig != nullptr, "Output parameter was nullptr");
	}

	Parser::~Parser()
	{
		delete m_pImpl;
	}

	auto Parser::Load(const Path& path) -> bool
	{
		const auto configFilePath = Impl::s_ConfigDirPath / path;

		m_pImpl->logger.Trace("Parsing configuration file: {}", configFilePath.string());

		if (!(std::filesystem::exists(configFilePath) && std::filesystem::is_regular_file(configFilePath))) {
			m_pImpl->logger.Error("Configuration file '{}' does not exist or is not a regular file.", configFilePath.string());
			return false;
		}

		auto currentSection = Section();
		auto currentSectionPath = std::string("/");
		auto sectionDirty = false;

		auto stream = std::ifstream(configFilePath);
		auto line = std::string();
		while (std::getline(stream, line)) {
			RemoveComments(line);
			Trim(line);

			if (line.empty()) {
				continue;
			}
			
			if (line[0] == '[') {
				if (sectionDirty) {
					m_pImpl->outConfig->Add(currentSectionPath, std::move(currentSection));
					sectionDirty = false;
				}

				if (!ParseSection(line, currentSectionPath)) {
					// TODO: Error handling
				}
			}
			else {
				if (ParseParameter(line, currentSection)) {
					sectionDirty = true;
				}
				else {
					// TODO: Error handling
				}
			}
		}

		if (sectionDirty) {
			m_pImpl->outConfig->Add(currentSectionPath, std::move(currentSection));
;		}

		return true;
	}

	auto Parser::ParseSection(const std::string & sectionStr, std::string& outSectionPath) -> bool
	{
		// TODO: Error handling (e.g. non-matching "[", "]")
		outSectionPath = sectionStr.substr(1, sectionStr.find_first_of(']') - 1);
		m_pImpl->logger.Trace("Parsed section: {}", outSectionPath);

		return true;
	}

	auto Parser::ParseParameter(const std::string& param, Section& outSection) -> bool
	{
		// TODO: Error handling

		const auto eqIdx = param.find_first_of('=');
		auto name = param.substr(0, eqIdx - 1);
		auto value = param.substr(eqIdx + 1, param.size() - eqIdx);

		Trim(name);
		Trim(value);

		if (value[0] == '"') {
			Trim(value, "\"");
		}

		m_pImpl->logger.Trace("Parsed variable: {} = {}", name, value);
		outSection.Add(std::move(name), std::move(value));

		return true;
	}
}

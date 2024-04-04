#include "Config/Configuration.hpp"

namespace Gaze::Config {
	auto Section::Add(std::string name, std::string value) -> void
	{
		m_ValuesMap.insert({ std::move(name), std::move(value) });
	}

	auto Configuration::Add(std::string path, Section section) -> void
	{
		m_SectionsMap.insert({ std::move(path), std::move(section) });
	}
}

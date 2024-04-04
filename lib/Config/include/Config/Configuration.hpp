#pragma once

#include <string>
#include <cstdlib>
#include <climits>
#include <unordered_map>

namespace Gaze::Config {
	template<typename T>
	struct Converter
	{
		auto operator()(const std::string& str, T& out) const -> bool
		{
			out = str;

			return true;
		}
	};

	template<>
	struct Converter<int>
	{
		auto operator()(const std::string& str, int& out) const -> bool
		{
			errno = 0; // strtol() modifies it in case of errors
			char* end;
			auto res = strtol(str.c_str(), &end, 10);

			// TODO: Handle the case where the value has trailing non-integer chars?

			if ((res == LONG_MIN || res == LONG_MAX) && errno == ERANGE) {
				return false;
			}

			if (res < std::numeric_limits<int>::min() || res > std::numeric_limits<int>::max()) {
				// Value too small/large for an int
				return false;
			}

			if (res == 0 && end == str.c_str()) {
				return false; // No conversion could be performed
			}

			out = int(res);

			return true;
		}
	};

	class Section
	{
	public:
		using Map = std::unordered_map<std::string, std::string>;

	public:
		auto Add(std::string name, std::string value) -> void;

		template<typename T, Converter TConverter = Converter<T>()>
		[[nodiscard]] auto Get(const std::string& name, T& out) -> bool
		{
			if (!m_ValuesMap.contains(name)) {
				return false;
			}

			return TConverter(m_ValuesMap[name], out);
		}

		[[nodiscard]] auto IsEmpty() const -> bool
		{
			return m_ValuesMap.empty();
		}

	private:
		std::unordered_map<std::string, std::string> m_ValuesMap;
	};

	class Configuration
	{
	public:
		using Map = std::unordered_map<std::string, Section>;

	public:
		auto Add(std::string path, Section section) -> void;

		template<typename T, Converter TConverter = Converter<T>()>
		[[nodiscard]] auto Get(const std::string& path, const std::string& name, T& out) -> bool
		{
			if (!m_SectionsMap.contains(path)) {
				return false;
			}

			return m_SectionsMap[path].Get<T, TConverter>(name, out);
		}

	private:
		Map m_SectionsMap;
	};
}

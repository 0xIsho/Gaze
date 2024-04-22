#pragma once

#include <string>
#include <cstdlib>
#include <limits>
#include <climits>
#include <unordered_map>

namespace Gaze::Config {
	/**
	 * @brief A generic converter that converts a string to a specified type.
	 *
	 * This struct acts as a callable object that attempts to convert
	 * a given string to a given type.
	 *
	 * This template should be specialzed for specific types.
	 *
	 * @tparam T The type to which the string should be converted.
	 */
	template<typename T>
	struct Converter
	{
		/**
		 * @brief Converts a string to the specified type.
		 *
		 * This function attempts to convert the given string to the specified
		 * type and assigns the result to the provided output parameter.
		 *
		 * Note that this operator attemps an implicit conversion which is
		 * likely *not* what you want.
		 *
		 * Speicialize this template for specific types.
		 * 
		 * @param str The string to be converted.
		 * @param out The output parameter where the result will be stored.
		 *
		 * @return true if the conversion was successful, false otherwise.
		 */
		auto operator()(const std::string& str, T& out) const -> bool
		{
			out = str;

			return true;
		}
	};

	/**
	 * @brief Specialization of the generic converter for converting strings to int.
	 *
	 * This struct provides a specialized `operator()` for converting a given string to an int
	 * and assigning the result to the provided output parameter.
	 */
	template<>
	struct Converter<int>
	{
		/**
		 * @brief Converts a string to an int.
		 *
		 * This function attempts to convert the given string to an int and assigns
		 * the result to the provided output parameter.
		 *
		 * @param str The string to be converted.
		 * @param out The output parameter where the result will be stored.
		 *
		 * @return true if the conversion was successful, false otherwise.
		 */
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

	/**
	 * @brief A configuration section. Maps variables to their respective values
	 *
	 * @see @ref Configuration
	 */
	class Section
	{
	public:
		using Map = std::unordered_map<std::string, std::string>;

	public:
		/**
		 * @brief Add a new Key-Value pair to the section
		 */
		auto Add(std::string name, std::string value) -> void;

		/**
		 * @brief Retieve a value from the section
		 *
		 * This function retrieves the value of the given variable from the section.
		 * The output parameter is *not* modified if the function fails.
		 *
		 * Example usage:
		 * 
		 * @code
		 * int x = 0;
		 * if (mySection.Get<int>("foo", x)) {
		 *   // Do something with x
		 * }
		 * else {
		 *   // Error! Either variable "foo" doesn't exist or its value could not be
		 *   // converted to an 'int'
		 * }
		 * @endcode
		 * 
		 * @tparam T The type of the stored value
		 * @tparam TConverter The @ref Converter instance to use for the string -> T conversion
		 *
		 * @param name The name of the variable
		 * @param out The output parameter in which the value will be stored
		 *
		 * @return true If the variable exists and was successfully converted to type T
		 *         false otherwise.
		 *
		 * @note Parameter 'out' is not modified on failure
		 */
		template<typename T, Converter TConverter = Converter<T>()>
		[[nodiscard]] auto Get(const std::string& name, T& out) -> bool
		{
			if (!m_ValuesMap.contains(name)) {
				return false;
			}

			return TConverter(m_ValuesMap[name], out);
		}

		/**
		 * @brief Check if the section is empty
		 */
		[[nodiscard]] auto IsEmpty() const -> bool
		{
			return m_ValuesMap.empty();
		}

	private:
		std::unordered_map<std::string, std::string> m_ValuesMap;
	};

	/**
	 * @brief A configuration object. Holds a collection of configuration sections
	 *
	 * Example configuration:
	 *
	 * @code
	 * [section1]
	 * foo = bar
	 *
	 * [section2]
	 * baz = qux
	 * @endcode
	 * 
	 * @see @ref Section
	 */
	class Configuration
	{
	public:
		using Map = std::unordered_map<std::string, Section>;

	public:
		/**
		 * @brief Add a new section to the configuration
		 *
		 * This function adds a new section to the configuration.
		 *
		 * @param path The path of the section. Freeform string for now. No special handling takes place
		 * @param section The section to add. Populate a section object then pass it to this function
		 */
		auto Add(std::string path, Section section) -> void;

		/**
		 * @brief Retrieve a value from the configuration object
		 *
		 * This function, similar to @ref Section::Get() retrieves a value from the
		 * configuration object.
		 *
		 * Example usage:
		 *
		 * Assuming the following configuration block:
		 * @code
		 * [/Path/To/Section]
		 * Foo = 10
		 * @endcode
		 * 
		 * @code
		 * int x = 0;
		 * if (myConfig.Get<int>("/Path/To/Section", "Foo", x)) {
		 *   // Success! x = 10
		 * }
		 * else {
		 *   // Error! Variable doesn't exist or could not be converted to an 'int'
		 * }
		 * @endcode
		 *
		 * @tparam T The type of the configuration variable's value
		 * @tparam TConverter The @ref Converter instance used to do the string -> T conversion
		 *
		 * @param path Section's path, the part in [square brackets] in the example above
		 * @param name The variable's name
		 *
		 * @return true If the variable exists and was successfully converted to 'T'
		 *         false otherwise.
		 *
		 * @note The output parameter is not modified in case of failure.
		 */
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

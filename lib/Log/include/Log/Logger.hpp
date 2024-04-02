#pragma once

#include <string>
#include <format>
#include <string_view>
#include <filesystem>

namespace Gaze::Log {
	class Logger
	{
		struct Impl;

	public:
		Logger(std::string name);
		Logger(const Logger&) = delete;
		Logger(Logger&& other) noexcept;
		~Logger();

		auto operator=(const Logger&) = delete;
		auto operator=(Logger& other) noexcept -> Logger&;
		
		auto Trace(std::string_view msg) -> void;
		auto Debug(std::string_view msg) -> void;
		auto Info(std::string_view msg) -> void;
		auto Warn(std::string_view msg) -> void;
		auto Error(std::string_view msg) -> void;
		auto Critical(std::string_view msg) -> void;

		template<typename...TArgs>
		auto Trace(const std::format_string<TArgs...> fmt, TArgs&&...args) -> void
		{
			Trace(std::vformat(fmt.get(), std::make_format_args(args...)));
		}

		template<typename...TArgs>
		auto Debug(const std::format_string<TArgs...> fmt, TArgs&&...args) -> void
		{
			Debug(std::vformat(fmt.get(), std::make_format_args(args...)));
		}

		template<typename...TArgs>
		auto Info(const std::format_string<TArgs...> fmt, TArgs&&...args) -> void
		{
			Info(std::vformat(fmt.get(), std::make_format_args(args...)));
		}

		template<typename...TArgs>
		auto Warn(const std::format_string<TArgs...> fmt, TArgs&&...args) -> void
		{
			Warn(std::vformat(fmt.get(), std::make_format_args(args...)));
		}

		template<typename...TArgs>
		auto Error(const std::format_string<TArgs...> fmt, TArgs&&...args) -> void
		{
			Error(std::vformat(fmt.get(), std::make_format_args(args...)));
		}

		template<typename...TArgs>
		auto Critical(const std::format_string<TArgs...> fmt, TArgs&&...args) -> void
		{
			Critical(std::vformat(fmt.get(), std::make_format_args(args...)));
		}

		auto Flush() -> void;

		static auto LogsDirectoryPath(const std::filesystem::path& path, bool mkdirIfMissing = true) -> void;

	private:
		Impl* m_pImpl = nullptr;
	};
}

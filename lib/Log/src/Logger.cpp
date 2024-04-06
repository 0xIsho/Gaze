#include "Log/Logger.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <cctype>
#include <memory>
#include <vector>
#include <functional>

namespace Gaze::Log {
	struct Logger::Impl
	{
		std::unique_ptr<spdlog::logger> handle;

		static std::filesystem::path s_LogsDirPath;
	};

	std::filesystem::path Logger::Impl::s_LogsDirPath;

	Logger::Logger(std::string name)
		: m_pImpl(new Impl())
	{
		spdlog::flush_on(spdlog::level::err);

		auto sinks = std::vector<spdlog::sink_ptr>();

		auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
#ifdef NDEBUG
		consoleSink->set_level(spdlog::level::info);
#endif
		sinks.emplace_back(std::move(consoleSink));
		if (!Impl::s_LogsDirPath.empty()) {
			auto filename = name;
			filename.erase(std::remove_if(filename.begin(), filename.end(), std::not_fn(isalnum)), filename.end());
			sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>((Impl::s_LogsDirPath / filename).string() + ".log"));
		}

		m_pImpl->handle = std::make_unique<spdlog::logger>(spdlog::logger(name, sinks.begin(), sinks.end()));
		m_pImpl->handle->set_level(spdlog::level::trace);
		m_pImpl->handle->set_pattern("%^[%C/%m/%d][%H:%M:%S:%e][%P::%t][%n::%L]:%$ %v");
	}

	Logger::Logger(Logger&& other) noexcept
	{
		if (this != &other) {
			m_pImpl = std::exchange(other.m_pImpl, nullptr);
		}
	}

	Logger::~Logger()
	{
		if (m_pImpl == nullptr) {
			return;
		}

		Flush();

		delete m_pImpl;
	}

	auto Logger::operator=(Logger&& other) noexcept -> Logger&
	{
		if (this != &other) {
			m_pImpl = std::exchange(other.m_pImpl, nullptr);
		}

		return *this;
	}
	
	auto Logger::Trace(std::string_view msg) -> void
	{
		m_pImpl->handle->trace(msg);
	}

	auto Logger::Debug(std::string_view msg) -> void
	{
		m_pImpl->handle->debug(msg);
	}

	auto Logger::Info(std::string_view msg) -> void
	{
		m_pImpl->handle->info(msg);
	}

	auto Logger::Warn(std::string_view msg) -> void
	{
		m_pImpl->handle->warn(msg);
	}

	auto Logger::Error(std::string_view msg) -> void
	{
		m_pImpl->handle->error(msg);
	}

	auto Logger::Critical(std::string_view msg) -> void
	{
		m_pImpl->handle->critical(msg);
	}

	auto Logger::Flush() -> void
	{
		m_pImpl->handle->flush();
	}

	auto Logger::LogsDirectoryPath(const std::filesystem::path& path, bool mkdirIfMissing) -> void
	{
		if (!std::filesystem::exists(path) && mkdirIfMissing) {
			if (!std::filesystem::create_directories(path)) {
				return;
			}
		}

		Impl::s_LogsDirPath = path;
	}
}

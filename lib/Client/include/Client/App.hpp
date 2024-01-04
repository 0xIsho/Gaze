#pragma once

#include "Core/Type.hpp"

namespace Gaze::Client {
	class App
	{
	public:
		enum class Status
		{
			Success = 0,
			Fail
		};

	public:
		App(int argc, char** argv) noexcept;
		virtual ~App();

		[[nodiscard]] auto Run()  noexcept -> Status;
		              auto Quit() noexcept -> void;

	private:
		virtual auto OnInit() -> Status = 0;
		virtual auto OnUpdate(F64 deltaTime) -> void = 0;
		virtual auto OnFixedUpdate(F64 /*deltaTime*/) -> void { }
		virtual auto OnShutdown() -> Status = 0;

	private:
		bool m_IsRunning = false;
	};

	auto CreateApp(int argc, char** argv) -> Mem::Unique<App>;
}

// The variable declaration at the end is so that a semicolon is required when
// the MACRO is used. Keeps things consistent.
#define GAZE_REGISTER_APP(TApp)                           \
	auto Gaze::Client::CreateApp(int argc, char** argv)   \
		-> ::Gaze::Mem::Unique<::Gaze::Client::App>       \
	{                                                     \
		return ::Gaze::Mem::MakeUnique<TApp>(argc, argv); \
	} int* _gaze_dummy ## __COUNTER__ = nullptr

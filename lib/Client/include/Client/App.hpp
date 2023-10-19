#pragma once

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
		App(int argc, char** argv);
		virtual ~App();

		[[nodiscard]] auto Run() -> Status;

	private:
		virtual auto OnInit() -> Status = 0;
		virtual auto OnUpdate() -> void = 0;
		virtual auto OnShutdown() -> Status = 0;

	private:
		bool m_IsRunning = false;
	};
}

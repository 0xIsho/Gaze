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
		virtual ~App() = default;

		[[nodiscard]] auto Run() -> Status;
	private:
	};
}

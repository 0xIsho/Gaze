#pragma once

#include "Input/KeyCode.hpp"

#include "Core/Type.hpp"

namespace Gaze {
	namespace WM {
		class Window;
	}

	namespace Input {
		class Handler
		{
		public:
			Handler(Mem::Shared<WM::Window> win);

			[[nodiscard]] auto IsKeyPressed(Key key) const -> bool;

		private:
			Mem::Shared<WM::Window> m_Win;
		};
	}
}

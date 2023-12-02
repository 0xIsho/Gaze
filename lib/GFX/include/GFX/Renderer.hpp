#pragma once

#include "Core/Type.hpp"

#include "GFX/API.hpp"

namespace Gaze::GFX {
	class Renderer
	{
	public:
		virtual ~Renderer() = default;

		virtual auto SetClearColor(F32 r, F32 g, F32 b, F32 a) -> void = 0;
		virtual auto Clear() -> void = 0;
		virtual auto Render() -> void = 0;
	};

	auto CreateRenderer() -> Mem::Unique<Renderer>;
}

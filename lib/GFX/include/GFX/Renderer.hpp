#pragma once

#include "Core/Type.hpp"

#include "GFX/API.hpp"
#include "GFX/Camera.hpp"
#include "GFX/Mesh.hpp"

#include "WM/Window.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <array>

namespace Gaze::GFX {
	using Vec3 = glm::vec3;

	class Renderer
	{
	public:
		struct RenderStats
		{
			I32 nDrawCalls;
		};

		enum class PrimitiveMode
		{
			Points,
			Lines,
			LineStrip,
			LineLoop,
			Triangles,
			TriangleStrip,
			TriangleFan
		};

		enum Buffer : U8
		{
			kColorBuffer   = 1 << 0,
			kDepthBuffer   = 1 << 1,
			kStencilBuffer = 1 << 2
		};

	public:
		Renderer(Mem::Shared<WM::Window> window) noexcept;
		virtual ~Renderer() = default;

		virtual auto SetClearColor(F32 r, F32 g, F32 b, F32 a)                  noexcept -> void = 0;
		virtual auto Clear(Buffer buffer = Buffer(kColorBuffer | kDepthBuffer)) noexcept -> void = 0;
		virtual auto Flush()                                                    noexcept -> void = 0;
		virtual auto Render()                                                   noexcept -> void = 0;
		virtual auto MakeContextCurrent()                                       noexcept -> void = 0;
		virtual auto Stats()                                                    noexcept -> RenderStats = 0;
		virtual auto SetViewport(I32 x, I32 y, I32 width, I32 height)           noexcept -> void = 0;
		virtual auto SetProjection(glm::mat4 projection)                        noexcept -> void = 0;
		virtual auto SetCamera(Mem::Shared<Camera> camera)                      noexcept -> void = 0;
		virtual auto DrawMesh(const Mesh& mesh, PrimitiveMode mode)                      -> void = 0;

	protected:
		[[nodiscard]] auto Window() const noexcept -> const WM::Window&;
		[[nodiscard]] auto Window()       noexcept -> WM::Window&;

	private:
		Mem::Shared<WM::Window> m_Window;
	};

	inline auto Renderer::Window() const noexcept -> const WM::Window&
	{
		return *m_Window.get();
	}
	inline auto Renderer::Window() noexcept -> WM::Window&
	{
		return *m_Window.get();
	}


	auto CreateRenderer(Mem::Shared<WM::Window> window) -> Mem::Unique<Renderer>;
}

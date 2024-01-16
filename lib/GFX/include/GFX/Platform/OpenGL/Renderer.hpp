#pragma once

#include "GFX/Renderer.hpp"

namespace Gaze::GFX::Platform::OpenGL {
	class Renderer : public GFX::Renderer
	{
		using GLID = U32;

		struct Impl;

	public:
		Renderer(Mem::Shared<WM::Window> window) noexcept;
		virtual ~Renderer();

		auto SetClearColor(F32 r, F32 g, F32 b, F32 a)        noexcept -> void override;
		auto Clear(Buffer buffer = kColorBuffer)              noexcept -> void override;
		auto Flush()                                          noexcept -> void override;
		auto Render()                                         noexcept -> void override;
		auto MakeContextCurrent()                             noexcept -> void override;
		auto Stats()                                          noexcept -> RenderStats override;
		auto SetViewport(I32 x, I32 y, I32 width, I32 height) noexcept -> void override;
		auto SetCamera(Mem::Shared<Camera> camera)            noexcept -> void override;
		auto DrawMesh(const Mesh& mesh, PrimitiveMode mode)            -> void override;
		auto DrawMesh(
			const Mesh& mesh,
			const Light lights[],
			I32 nLights,
			PrimitiveMode mode
		) -> void override;
		auto SubmitObject(const Object& object, PrimitiveMode mode)    -> void override;
		auto SubmitObject(
			const Object& object,
			const struct Light lights[],
			I32 nLights,
			PrimitiveMode mode
		) -> void override;

	private:
		Impl* m_pImpl{ nullptr };
	};
}

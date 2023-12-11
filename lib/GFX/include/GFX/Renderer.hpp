#pragma once

#include "Core/Type.hpp"

#include "GFX/API.hpp"
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

	public:
		Renderer(Mem::Shared<WM::Window> window);
		virtual ~Renderer() = default;

		virtual auto SetColor(F32 r, F32 g, F32 b, F32 a) -> void = 0;
		virtual auto SetClearColor(F32 r, F32 g, F32 b, F32 a) -> void = 0;
		virtual auto Clear() -> void = 0;
		virtual auto Render() -> void = 0;
		virtual auto SetViewport(I32 x, I32 y, I32 width, I32 height) -> void = 0;
		virtual auto SetProjection(glm::mat4 projection) -> void = 0;

		virtual auto DrawMesh(const Mesh& mesh, PrimitiveMode mode) -> void = 0;

		/* ********** 2D Primitive Rendering Functions ********** */
		// Coordinates are NDC [-1.0F,1.0F]!

		virtual auto DrawPoint(Vec3 p) -> void = 0;
		virtual auto DrawLine(Vec3 start, Vec3 end) -> void = 0;
		virtual auto DrawTri(const std::array<Vec3, 3>& ps) -> void = 0;
		virtual auto FillTri(const std::array<Vec3, 3>& ps) -> void = 0;
		/**********************************************************/

	protected:
		[[nodiscard]]
		auto Window() const -> const WM::Window& { return *m_Window.get(); }
		[[nodiscard]]
		auto Window() -> WM::Window& { return *m_Window.get(); }

	private:
		Mem::Shared<WM::Window> m_Window;
	};

	auto CreateRenderer(Mem::Shared<WM::Window> window) -> Mem::Unique<Renderer>;
}

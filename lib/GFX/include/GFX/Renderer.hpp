#pragma once

#include "Core/Type.hpp"

#include "GFX/API.hpp"
#include "GFX/Camera.hpp"
#include "GFX/Mesh.hpp"
#include "GFX/Object.hpp"

#include "WM/Window.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <array>

namespace Gaze::GFX {
	using Vec3 = glm::vec3;

	/**
	 * @brief The Renderer class
	 *
	 * The Renderer class is the base class for all renderers.
	 * It provides a common interface for all renderers.
	 */
	class Renderer
	{
	public:
		/**
		 * @brief The RenderStats struct
		 *
		 * The RenderStats struct contains statistics about the rendering process.
		 */
		struct RenderStats
		{
			I32 nDrawCalls;
		};

		/**
		 * @brief Defines the primitive mode for rendering.
		 */
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

		/**
		 * @brief Used to specify which buffers to clear.
		 *
		 * This enum is used to specify which buffers to clear.
		 * It is used in the Clear() method.
		 */
		enum Buffer : U8
		{
			kColorBuffer   = 1 << 0,
			kDepthBuffer   = 1 << 1,
			kStencilBuffer = 1 << 2
		};

	public:
		/**
		 * @brief Construct a new renderer object
		 *
		 * The constructor initializes the graphics context and sets up the required
		 * resources for rendering.
		 *
		 * @todo It doesn't report failures
		 * 
		 * @param window The window to render to
		 */
		Renderer(Shared<WM::Window> window) noexcept;
		/**
		 * @brief Destroy the renderer object
		 *
		 * The destructor cleans up the graphics context and releases the resources
		 * used for rendering.
		 */
		virtual ~Renderer() = default;

		/**
		 * @brief Set the clear color
		 *
		 * @param r The red component of the clear color
		 * @param g The green component of the clear color
		 * @param b The blue component of the clear color
		 * @param a The alpha component of the clear color
		 */
		virtual auto SetClearColor(F32 r, F32 g, F32 b, F32 a) noexcept -> void = 0;
		/**
		 * @brief Clear the given buffers
		 *
		 * @param buffer The buffers to clear
		 */
		virtual auto Clear(Buffer buffer = Buffer(kColorBuffer | kDepthBuffer)) noexcept -> void = 0;
		/**
		 * @brief Flush the command buffer
		 */
		virtual auto Flush() noexcept -> void = 0;
		/**
		 * @brief Render the scene
		 *
		 * This function flushes the command buffer and swaps the back and front
		 * buffers.
		 */
		virtual auto Render() noexcept -> void = 0;
		/**
		 * @brief Make this renderer's context current
		 *
		 * This function must be called before calling any other functions. If
		 * there was no current context when the Renderer object was created,
		 * it makes it's own context current. New renderer objects must make
		 * their context current before rendering.
		 */
		virtual auto MakeContextCurrent() noexcept -> void = 0;
		/**
		 * @brief Get the current render stats
		 *
		 * @return The current render stats
		 */
		virtual auto Stats() noexcept -> RenderStats = 0;
		/**
		 * @brief Set the render viewport
		 *
		 * @param x The x coordinate of the viewport
		 * @param y The y coordinate of the viewport
		 * @param width The width of the viewport
		 * @param height The height of the viewport
		 */
		virtual auto SetViewport(I32 x, I32 y, I32 width, I32 height) noexcept -> void = 0;
		/**
		 * @brief Set the camera
		 *
		 * @param camera The camera to use
		 */
		virtual auto SetCamera(Shared<Camera> camera) noexcept -> void = 0;
		[[deprecated("Use SubmitObject()")]]
		virtual auto DrawMesh(const Mesh& mesh, PrimitiveMode mode) -> void = 0;
		[[deprecated("Use SubmitObject()")]]
		virtual auto DrawMesh(
			const Mesh& mesh,
			const struct Light lights[],
			I32 nLights,
			PrimitiveMode mode
		) -> void = 0;
		/**
		 * @brief Submit an object for rendering
		 *
		 * @param object The object to submit
		 * @param mode The primitive mode to use
		 */
		virtual auto SubmitObject(const Object& object, PrimitiveMode mode) -> void = 0;
		/**
		 * @brief Submit an object for rendering
		 *
		 * @param object The object to submit
		 * @param lights The lights to use
		 * @param nLights The number of lights
		 * @param mode The primitive mode to use
		 */
		virtual auto SubmitObject(
			const Object& object,
			const struct Light lights[],
			I32 nLights,
			PrimitiveMode mode
		) -> void = 0;

	protected:
		[[nodiscard]] auto Window() const noexcept -> const WM::Window&;
		[[nodiscard]] auto Window()       noexcept -> WM::Window&;

	private:
		Shared<WM::Window> m_Window;
	};

	inline auto Renderer::Window() const noexcept -> const WM::Window&
	{
		return *m_Window.get();
	}
	inline auto Renderer::Window() noexcept -> WM::Window&
	{
		return *m_Window.get();
	}


	auto CreateRenderer(Shared<WM::Window> window) -> Unique<Renderer>;
}

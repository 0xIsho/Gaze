#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <utility>

namespace Gaze::GFX {
	/**
	 * @brief Base class for all cameras.
	 */
	class Camera
	{
	public:
		virtual ~Camera() = default;

		/**
		 * @brief Computes the view matrix for the camera.
		 *
		 * @return The view matrix.
		 */
		[[nodiscard]]
		auto ComputeViewMatrix()         const noexcept -> glm::mat4;
		/**
		 * @brief Move the camera in the given direction.
		 *
		 * @param dir The direction to move the camera.
		 */
		auto Move(glm::vec3 dir)               noexcept -> void;

		/**
		 * @brief Return the current position of the camera.
		 *
		 * @return The current position of the camera.
		 */
		[[nodiscard]]
		auto Position()                  const noexcept -> const glm::vec3&;
		/**
		 * @brief Set the position of the camera.
		 *
		 * @param vec The new position of the camera.
		 */
		[[nodiscard]]
		auto SetPosition(glm::vec3 vec)        noexcept -> void;

		/**
		 * @brief Return the front vector of the camera.
		 *
		 * @return The front vector of the camera.
		 */
		[[nodiscard]]
		auto Front()                     const noexcept -> const glm::vec3&;
		/**
		 * @brief Set the front vector of the camera.
		 */
		auto SetFront(glm::vec3 vec)           noexcept -> void;

		/**
		 * @brief Return the up vector of the camera.
		 *
		 * @return The up vector of the camera.
		 */
		[[nodiscard]]
		auto Up()                        const noexcept -> const glm::vec3&;

		/**
		 * @brief Return the right vector of the camera.
		 *
		 * @return The right vector of the camera.
		 */
		[[nodiscard]]
		auto Right()                     const noexcept -> glm::vec3;

		/**
		 * @breief Compute the projection matrix for the camera.
		 *
		 * Specific camera types should override this method to compute the projection
		 * matrix.
		 *
		 * @return The projection matrix.
		 *
		 * @see PerspectiveCamera
		 * @see OrthographicCamera
		 */
		[[nodiscard]]
		virtual auto ComputeProjectionMatrix() const noexcept -> glm::mat4 = 0;

	private:
		glm::vec3 m_Position { .0F, .0F,  1.F };
		glm::vec3 m_Front    { .0F, .0F, -1.F };
		glm::vec3 m_Up       { .0F, 1.F,  .0F };
	};

	/**
	 * @brief A camera that uses a perspective projection.
	 */
	class PerspectiveCamera : public Camera
	{
	public:
		/**
		 * @brief Construct a new PerspectiveCamera object.
		 *
		 * @param fovRadians The field of view in radians.
		 * @param aspect The aspect ratio.
		 * @param zNear The near clipping plane.
		 * @param zFar The far clipping plane.
		 *//
		PerspectiveCamera(float fovRadians, float aspect, float zNear, float zFar) noexcept;

		/**
		 * @brief Compute the projection matrix for the camera.
		 *
		 * @return The projection matrix.
		 */
		[[nodiscard]] auto ComputeProjectionMatrix() const noexcept -> glm::mat4 override;

	private:
		float m_FovRadians;
		float m_Aspect;
		float m_ZNear;
		float m_ZFar;
	};

	/**
	 * @brief A camera that uses an orthographic projection.
	 */
	class OrthographicCamera : public Camera
	{
	public:
		/**
		 * @brief Construct a new OrthographicCamera object.
		 *
		 * @param left The left clipping plane.
		 * @param right The right clipping plane.
		 * @param bottom The bottom clipping plane.
		 * @param top The top clipping plane.
		 * @param zNear The near clipping plane.
		 * @param zFar The far clipping plane.
		 *//
		OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar) noexcept;

		/**
		 * @brief Compute the projection matrix for the camera.
		 *
		 * @return The projection matrix.
		 */
		[[nodiscard]] auto ComputeProjectionMatrix() const noexcept -> glm::mat4 override;

	private:
		float m_Left;
		float m_Right;
		float m_Bottom;
		float m_Top;
		float m_ZNear;
		float m_ZFar;
	};

	inline auto Camera::Position() const noexcept -> const glm::vec3&
	{
		return m_Position;
	}

	inline auto Camera::SetPosition(glm::vec3 vec) noexcept -> void
	{
		m_Position = std::move(vec);
	}

	inline auto Camera::Front() const noexcept -> const glm::vec3&
	{
		return m_Front;
	}

	inline auto Camera::SetFront(glm::vec3 vec) noexcept -> void
	{
		m_Front = std::move(vec);
	}

	inline auto Camera::Up() const noexcept -> const glm::vec3&
	{
		return m_Up;
	}

	inline auto Camera::Right() const noexcept -> glm::vec3
	{
		return glm::normalize(glm::cross(m_Front, m_Up));
	}

	inline PerspectiveCamera::PerspectiveCamera(float fovRadians, float aspect, float zNear, float zFar) noexcept
		: m_FovRadians(fovRadians)
		, m_Aspect(aspect)
		, m_ZNear(zNear)
		, m_ZFar(zFar)
	{
	}

	inline auto PerspectiveCamera::ComputeProjectionMatrix() const noexcept -> glm::mat4
	{
		return glm::perspective(m_FovRadians, m_Aspect, m_ZNear, m_ZFar);
	}

	inline OrthographicCamera::OrthographicCamera(
		float left,
		float right,
		float bottom,
		float top,
		float zNear,
		float zFar
	) noexcept
		: m_Left(left)
		, m_Right(right)
		, m_Bottom(bottom)
		, m_Top(top)
		, m_ZNear(zNear)
		, m_ZFar(zFar)
	{
	}

	inline auto OrthographicCamera::ComputeProjectionMatrix() const noexcept -> glm::mat4
	{
		return glm::ortho(m_Left, m_Right, m_Bottom, m_Top, m_ZNear, m_ZFar);
	}
}

#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <utility>

namespace Gaze::GFX {
	class Camera
	{
	public:
		virtual ~Camera() = default;

		[[nodiscard]]
		auto ComputeViewMatrix()         const noexcept -> glm::mat4;
		auto Move(glm::vec3 dir)               noexcept -> void;

		[[nodiscard]]
		auto Position()                  const noexcept -> const glm::vec3&;
		auto SetPosition(glm::vec3 vec)        noexcept -> void;

		[[nodiscard]]
		auto Front()                     const noexcept -> const glm::vec3&;
		auto SetFront(glm::vec3 vec)           noexcept -> void;

		[[nodiscard]]
		auto Up()                        const noexcept -> const glm::vec3&;

		[[nodiscard]]
		auto Right()                     const noexcept -> glm::vec3;

		[[nodiscard]]
		virtual auto ComputeProjectionMatrix() const noexcept -> glm::mat4 = 0;

	private:
		glm::vec3 m_Position { .0F, .0F,  1.F };
		glm::vec3 m_Front    { .0F, .0F, -1.F };
		glm::vec3 m_Up       { .0F, 1.F,  .0F };
	};

	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float fovRadians, float aspect, float zNear, float zFar) noexcept;

		[[nodiscard]] auto ComputeProjectionMatrix() const noexcept -> glm::mat4 override;

	private:
		float m_FovRadians;
		float m_Aspect;
		float m_ZNear;
		float m_ZFar;
	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar) noexcept;

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

#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <utility>

namespace Gaze::GFX {
	class Camera
	{
	public:
		[[nodiscard]]
		auto ComputeViewMatrix()        const noexcept -> glm::mat4;

		auto Move(glm::vec3 dir)              noexcept -> void;

		[[nodiscard]]
		auto Position()                 const noexcept -> const glm::vec3&;
		auto SetPosition(glm::vec3 vec)       noexcept -> void;
		[[nodiscard]]
		auto Front()                    const noexcept -> const glm::vec3&;
		auto SetFront(glm::vec3 vec)          noexcept -> void;
		[[nodiscard]]
		auto Up()                       const noexcept -> const glm::vec3&;
		[[nodiscard]]
		auto Right()                    const noexcept -> glm::vec3;

	private:
		glm::vec3 m_Position { .0F, .0F,  1.F };
		glm::vec3 m_Front    { .0F, .0F, -1.F };
		glm::vec3 m_Up       { .0F, 1.F,  .0F };
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
}

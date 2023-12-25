#pragma once

#include "Core/Type.hpp"

#include "Debug/Assert.hpp"

#include "glad/gl.h"

#include <type_traits>

namespace Gaze::GFX::Platform::OpenGL::Objects {
	template<typename T>
	class Object
	{
	public:
		using GLID = U32;

		friend auto swap(Object& first, Object& second) -> void
		{
			using std::swap;

			swap(first.m_ID, second.m_ID);
		}

	public:
		Object(const Object&) = delete;
		Object(Object&& other) noexcept
			: Object(0)
		{
			GAZE_ASSERT(this != &other, "Self-Assignment");
			swap(*this, other);
		}

		~Object()
		{
			static_cast<T*>(this)->Release(m_ID);
			GAZE_ASSERT(m_ID == 0, "ID not reset to 0 after GL object release. (Erroneous Release() implementation)");
		}

		auto operator=(const Object&) -> Object& = delete;
		auto operator=(Object&& other) noexcept -> Object&
		{
			GAZE_ASSERT(this != &other, "Self-Assignment");

			static_cast<T*>(this)->Release(m_ID);
			swap(*this, other);
		}

		[[nodiscard]]
		auto ID() const -> GLID
		{
			return m_ID;
		}

	protected:
		explicit Object(GLID id)
			: m_ID(id)
		{
		}

	private:
		GLID m_ID;
	};
}

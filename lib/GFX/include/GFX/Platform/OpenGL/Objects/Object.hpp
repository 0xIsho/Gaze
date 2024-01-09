#pragma once

#include "Core/Type.hpp"
#include "Core/PlatformUtils.hpp"

#include "Debug/Assert.hpp"

#include "glad/gl.h"

#include <type_traits>

namespace Gaze::GFX::Platform::OpenGL::Objects {
	enum class BufferUsage
	{
		StaticDraw,
		StaticCopy,
		StaticRead,

		DynamicDraw,
		DynamicCopy,
		DynamicRead,

		StreamDraw,
		StreamCopy,
		StreamRead,
	};

	inline constexpr auto ToGLBufferUsage(BufferUsage usage) -> GLenum {
		switch (usage) {
		case BufferUsage::StaticDraw:  return GL_STATIC_DRAW;
		case BufferUsage::StaticCopy:  return GL_STATIC_COPY;
		case BufferUsage::StaticRead:  return GL_STATIC_READ;
		case BufferUsage::DynamicDraw: return GL_DYNAMIC_DRAW;
		case BufferUsage::DynamicCopy: return GL_DYNAMIC_COPY;
		case BufferUsage::DynamicRead: return GL_DYNAMIC_READ;
		case BufferUsage::StreamDraw:  return GL_STREAM_DRAW;
		case BufferUsage::StreamCopy:  return GL_STREAM_COPY;
		case BufferUsage::StreamRead:  return GL_STREAM_READ;
		}

		GAZE_UNREACHABLE();
	}

	template<typename T>
	class Object
	{
	public:
		using GLID = U32;

		friend auto swap(Object& first, Object& second) noexcept -> void
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
			T::Release(m_ID);
			GAZE_ASSERT(m_ID == 0, "ID not reset to 0 after GL object release. (Erroneous Release() implementation)");
		}

		auto operator=(const Object&) -> Object& = delete;
		auto operator=(Object&& other) noexcept -> Object&
		{
			GAZE_ASSERT(this != &other, "Self-Assignment");

			T::Release(m_ID);
			swap(*this, other);

			return *this;
		}

		[[nodiscard]]
		auto ID() const noexcept -> GLID
		{
			return m_ID;
		}

	protected:
		explicit Object(GLID id) noexcept
			: m_ID(id)
		{
		}

	private:
		GLID m_ID;
	};
}

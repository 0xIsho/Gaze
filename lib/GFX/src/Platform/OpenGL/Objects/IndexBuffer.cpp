#include "GFX/Platform/OpenGL/Objects/IndexBuffer.hpp"

namespace Gaze::GFX::Platform::OpenGL::Objects {
	IndexBuffer::IndexBuffer() noexcept
		: Object([] { GLID id; glCreateBuffers(1, &id); return id; }())
	{
	}

	IndexBuffer::IndexBuffer(const void* data, I64 size, BufferUsage usage /*= BufferUsage::StaticDraw*/) noexcept
		: IndexBuffer()
	{
		glNamedBufferData(ID(), size, data, ToGLBufferUsage(usage));
	}

	auto IndexBuffer::Release(GLID& id) noexcept -> void
	{
		glDeleteBuffers(1, &id);
		id = 0;
	}

	auto IndexBuffer::Bind() const noexcept -> void
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID());
	}

	auto IndexBuffer::Upload(const void* data, I64 size, I64 offset) noexcept -> void
	{
		glNamedBufferSubData(ID(), offset, size, data);
	}
}

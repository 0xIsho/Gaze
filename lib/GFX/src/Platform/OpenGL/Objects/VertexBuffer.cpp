#include "GFX/Platform/OpenGL/Objects/VertexBuffer.hpp"

#include "glad/gl.h"

namespace Gaze::GFX::Platform::OpenGL::Objects {
	VertexBuffer::VertexBuffer()
		: Object([] { GLID id; glCreateBuffers(1, &id); return id; }())
	{
	}

	VertexBuffer::VertexBuffer(const void* data, I64 size, BufferUsage usage /*= BufferUsage::StaticDraw*/)
		: VertexBuffer()
	{
		glNamedBufferData(ID(), size, data, ToGLBufferUsage(usage));
	}

	auto VertexBuffer::Bind(U32 bindingIndex, IPtr offset, I32 stride) const -> void
	{
		glBindVertexBuffer(bindingIndex, ID(), offset, stride);
	}

	auto VertexBuffer::Release(GLID& id) -> void
	{
		glDeleteBuffers(1, &id);
		id = 0;
	}

	auto VertexBuffer::Upload(const void* data, I64 size, I64 offset) -> void
	{
		glNamedBufferSubData(ID(), offset, size, data);
	}
}

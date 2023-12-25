#include "GFX/Platform/OpenGL/Objects/VertexBuffer.hpp"

#include "glad/gl.h"

namespace Gaze::GFX::Platform::OpenGL::Objects {
	VertexBuffer::VertexBuffer()
		: Object([] { GLID id; glGenBuffers(1, &id); return id; }())
	{
	}

	VertexBuffer::VertexBuffer(const void* data, I64 size, BufferUsage usage /*= BufferUsage::StaticDraw*/)
		: VertexBuffer()
	{
		Bind();

		glBufferData(
			GL_ARRAY_BUFFER,
			size,
			data,
			ToGLBufferUsage(usage)
		);
	}

	auto VertexBuffer::Bind() -> void
	{
		glBindBuffer(GL_ARRAY_BUFFER, ID());
	}

	auto VertexBuffer::Release(GLID& id) -> void
	{
		glDeleteBuffers(1, &id);
		id = 0;
	}
}

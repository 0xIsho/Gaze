#include "GFX/Platform/OpenGL/Objects/IndexBuffer.hpp"

namespace Gaze::GFX::Platform::OpenGL::Objects {
	IndexBuffer::IndexBuffer()
		: Object([] { GLID id; glGenBuffers(1, &id); return id; }())
	{
	}

	IndexBuffer::IndexBuffer(const void* data, I64 size, BufferUsage usage /*= BufferUsage::StaticDraw*/)
		: IndexBuffer()
	{
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, ToGLBufferUsage(usage));
	}

	auto IndexBuffer::Release(GLID& id) -> void
	{
		glDeleteBuffers(1, &id);
		id = 0;
	}

	auto IndexBuffer::Bind() const -> void
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID());
	}
}

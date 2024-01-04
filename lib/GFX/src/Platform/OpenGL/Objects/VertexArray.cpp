#include "GFX/Platform/OpenGL/Objects/VertexArray.hpp"

#include <utility>

namespace Gaze::GFX::Platform::OpenGL::Objects {
	VertexArray::VertexArray() noexcept
		: Object([] { GLID id; glCreateVertexArrays(1, &id); return id; }())
	{
	}

	auto VertexArray::Release(GLID& id) noexcept -> void
	{
		glDeleteVertexArrays(1, &id);
		id = 0;
	}

	auto VertexArray::Bind() const noexcept -> void
	{
		glBindVertexArray(ID());
	}

	auto VertexArray::BindVertexBuffer(
		VertexBuffer* buffer,
		BufferBinding binding,
		Offset offset,
		Stride stride
	) noexcept -> void
	{
		glVertexArrayVertexBuffer(ID(), binding.Value(), buffer->ID(), offset.Value(), stride.Value());
	}

	static auto ToOpenGLType(VertexArray::Layout::DataType type) noexcept -> GLenum
	{
		using DataType = VertexArray::Layout::DataType;

		switch (type) {
		case DataType::Byte:      return GL_BYTE;
		case DataType::Short:     return GL_SHORT;
		case DataType::Int:       return GL_INT;
		case DataType::Fixed:     return GL_FIXED;
		case DataType::Float:     return GL_FLOAT;
		case DataType::HalfFloat: return GL_HALF_FLOAT;
		case DataType::Double:    return GL_DOUBLE;
		}

#if GAZE_COMPILER_CLANG == 1 || GAZE_COMPILER_GNU == 1
		__builtin_unreachable();
#else
#	error "__builtin_unreachable() equivalent missing"
#endif
	}

	auto VertexArray::SetLayout(std::initializer_list<Layout> layout) noexcept -> void
	{
		for (auto i = 0UL; i < layout.size(); i++) {
			const auto& elem = *(layout.begin() + i);
			glVertexArrayAttribFormat(
				ID(),
				GLuint(i),
				elem.componentCount.Value(),
				ToOpenGLType(elem.type),
				elem.normalized.Value() ? GL_TRUE : GL_FALSE,
				elem.relativeOffset.Value()
			);
			glEnableVertexArrayAttrib(ID(), GLuint(i));
			glVertexArrayAttribBinding(ID(), GLuint(i), elem.bufferBinding.Value());
		}
	}

	auto VertexArray::SetIndexBuffer(IndexBuffer* buffer) noexcept -> void
	{
		glVertexArrayElementBuffer(ID(), buffer->ID());
	}
}

#pragma once

#include "Object.hpp"

namespace Gaze::GFX::Platform::OpenGL::Objects {
	class IndexBuffer : public Object<IndexBuffer>
	{
	public:
		IndexBuffer();
		IndexBuffer(const void* data, I64 size, BufferUsage usage = BufferUsage::StaticDraw);
		static auto Release(GLID& id) -> void;

		auto Bind() const -> void;
	};
}

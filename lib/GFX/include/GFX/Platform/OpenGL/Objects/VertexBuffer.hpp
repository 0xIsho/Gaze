#pragma once

#include "Object.hpp"

namespace Gaze::GFX::Platform::OpenGL::Objects {
	class VertexBuffer : public Object<VertexBuffer>
	{
	public:
		VertexBuffer();
		VertexBuffer(const void* data,I64 size,BufferUsage usage = BufferUsage::StaticDraw);
		static auto Release(GLID& id) -> void;

		auto Bind() -> void;
	};
}

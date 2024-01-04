#pragma once

#include "Object.hpp"

namespace Gaze::GFX::Platform::OpenGL::Objects {
	class VertexBuffer : public Object<VertexBuffer>
	{
	public:
		VertexBuffer() noexcept;
		VertexBuffer(const void* data, I64 size, BufferUsage usage = BufferUsage::StaticDraw) noexcept;
		static auto Release(GLID& id) noexcept -> void;

		auto Bind(U32 bindingIndex, IPtr offset, I32 stride) const noexcept -> void;
		auto Upload(const void* data, I64 size, I64 offset)        noexcept -> void;
	};
}

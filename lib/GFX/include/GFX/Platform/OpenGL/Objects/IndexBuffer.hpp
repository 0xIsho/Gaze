#pragma once

#include "Object.hpp"

namespace Gaze::GFX::Platform::OpenGL::Objects {
	class IndexBuffer : public Object<IndexBuffer>
	{
	public:
		IndexBuffer() noexcept;
		IndexBuffer(const void* data, I64 size, BufferUsage usage = BufferUsage::StaticDraw) noexcept;
		static auto Release(GLID& id) noexcept -> void;

		auto Bind() const noexcept -> void;

		auto Upload(const void* data, I64 size, I64 offset) noexcept -> void;
	};
}

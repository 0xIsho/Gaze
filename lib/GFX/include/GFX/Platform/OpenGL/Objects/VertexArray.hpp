#pragma once

#include "GFX/Platform/OpenGL/Objects/Object.hpp"
#include "GFX/Platform/OpenGL/Objects/IndexBuffer.hpp"
#include "GFX/Platform/OpenGL/Objects/VertexBuffer.hpp"

#include "Core/Type.hpp"

#include <initializer_list>

namespace Gaze::GFX::Platform::OpenGL::Objects {
	class VertexArray : public Object<VertexArray>
	{
	public:
		using BufferBinding = ValueWrapper<U32, struct BufferBindingTag>;
		using Offset        = ValueWrapper<U32, struct OffsetTag>;
		using Stride        = ValueWrapper<I32, struct StrideTag>;

	public:
		struct Layout
		{
			using BufferBinding  = VertexArray::BufferBinding;
			using ComponentCount = ValueWrapper<I32, struct ComponentCountTag>;
			using Normalized     = ValueWrapper<bool>;
			using RelativeOffset = ValueWrapper<U32, struct RelativeOffsetTag>;

			enum class DataType
			{
				Byte,
				Short,
				Int,
				Fixed,
				Float,
				HalfFloat,
				Double,
			};

			BufferBinding  bufferBinding;
			ComponentCount componentCount;
			DataType       type;
			Normalized     normalized;
			RelativeOffset relativeOffset;
		};

	public:
		VertexArray() noexcept;
		static auto Release(GLID& id) noexcept -> void;

		auto Bind() const noexcept -> void;
		auto BindVertexBuffer(
			VertexBuffer* buffer,
			BufferBinding binding,
			Offset offset,
			Stride stride)
		noexcept -> void;

		auto SetLayout(std::initializer_list<Layout> layout) noexcept -> void;
		auto SetIndexBuffer(IndexBuffer* buffer)             noexcept -> void;
	};
}

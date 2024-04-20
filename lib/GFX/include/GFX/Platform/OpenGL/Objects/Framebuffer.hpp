#pragma once

#include "Object.hpp"

#include "Core/Type.hpp"

#include "glad/gl.h"

namespace Gaze::GFX::Platform::OpenGL::Objects {
	class FramebufferAttachment : public Object<FramebufferAttachment>
	{
	public:
		FramebufferAttachment(I32 width, I32 height);
		static auto Release(GLID& id) noexcept -> void;
	};

	class Renderbuffer : public Object<Renderbuffer>
	{
	public:
		Renderbuffer(I32 width, I32 height);
		static auto Release(GLID& id) noexcept -> void;
	};

	class Framebuffer : public Object<Framebuffer>
	{
	public:
		Framebuffer(I32 width, I32 height);
		static auto Release(GLID& id) noexcept -> void;

		auto Bind() const noexcept -> void;

		auto ColorAttachmentID() const noexcept -> GLID { return m_ColorAttachment.ID(); }

		static auto Unbind() noexcept -> void;

	private:
		FramebufferAttachment m_ColorAttachment;
		Renderbuffer m_DepthStencilAttachment;
	};
}

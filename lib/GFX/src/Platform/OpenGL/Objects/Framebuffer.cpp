#include "GFX/Platform/OpenGL/Objects/Framebuffer.hpp"


#include "Debug/Assert.hpp"

namespace Gaze::GFX::Platform::OpenGL::Objects {
	FramebufferAttachment::FramebufferAttachment(I32 width, I32 height)
		: Object([] { GLID id; glCreateTextures(GL_TEXTURE_2D, 1, &id); return id; }())
	{
		glTextureParameteri(ID(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(ID(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureStorage2D(ID(), 1, GL_RGB8, width, height);
	}

	auto FramebufferAttachment::Release(GLID& id) noexcept -> void
	{
		glDeleteTextures(1, &id);
	}

	Renderbuffer::Renderbuffer(I32 width, I32 height)
		: Object([] { GLID id; glCreateRenderbuffers(1, &id); return id; }())
	{
		glNamedRenderbufferStorage(ID(), GL_DEPTH24_STENCIL8, width, height);
	}

	auto Renderbuffer::Release(GLID& id) noexcept -> void
	{
		glDeleteRenderbuffers(1, &id);
	}

	Framebuffer::Framebuffer(I32 width, I32 height)
		: Object([]{ GLID id; glCreateFramebuffers(1, &id); return id; }())
		, m_ColorAttachment(width, height)
		, m_DepthStencilAttachment(width, height)
	{
		glNamedFramebufferTexture(ID(), GL_COLOR_ATTACHMENT0, m_ColorAttachment.ID(), 0);
		glNamedFramebufferRenderbuffer(ID(), GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilAttachment.ID());

		GAZE_ASSERT(glCheckNamedFramebufferStatus(ID(), GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer incomplete");
	}

	auto Framebuffer::Release(GLID& id) noexcept -> void
	{
		glDeleteFramebuffers(1, &id);
		id = 0;
	}

	auto Framebuffer::Bind() const noexcept -> void
	{
		glBindFramebuffer(GL_FRAMEBUFFER, ID());
	}

	auto Framebuffer::Unbind() noexcept -> void
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

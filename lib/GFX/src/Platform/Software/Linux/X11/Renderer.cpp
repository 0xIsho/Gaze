#include "GFX/Platform/Software/Linux/X11/Renderer.hpp"

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#undef GLFW_EXPOSE_NATIVE_X11

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <limits>

namespace Gaze::GFX::Platform::Software::Linux::X11 {
	struct Renderer::Impl
	{
		Display* display;
		XID xwindow;
		GC gc;
		Pixmap pixmap;
		XColor clearcolor;
	};

	Renderer::Renderer(Mem::Shared<WM::Window> window)
		: GFX::Renderer(std::move(window))
		, m_pImpl(new Impl())
	{
		m_pImpl->display = glfwGetX11Display();
		m_pImpl->xwindow = glfwGetX11Window(static_cast<GLFWwindow*>(Window().Handle()));
		m_pImpl->gc = XCreateGC(m_pImpl->display, m_pImpl->xwindow, 0, nullptr);
		m_pImpl->pixmap = XCreatePixmap(
			m_pImpl->display,
			m_pImpl->xwindow,
			static_cast<unsigned>(Window().Width()),
			static_cast<unsigned>(Window().Height()),
			DefaultDepth(m_pImpl->display, DefaultScreen(m_pImpl->display))
		);
	}

	Renderer::~Renderer()
	{
		XFreeGC(m_pImpl->display, m_pImpl->gc);
		XFreePixmap(m_pImpl->display, m_pImpl->pixmap);
		XFreeColors(
			m_pImpl->display,
			DefaultColormap(m_pImpl->display, DefaultScreen(m_pImpl->display)),
			&m_pImpl->clearcolor.pixel,
			1,
			0
		);
		delete m_pImpl;
	}

	auto Renderer::SetClearColor(F32 r, F32 g, F32 b, F32 a) -> void
	{
		// Xlib stores colors in the 0-UINT16_MAX range, so scale as necessary
		m_pImpl->clearcolor.red = static_cast<U16>(r * std::numeric_limits<U16>::max() + .5F);
		m_pImpl->clearcolor.green = static_cast<U16>(g * std::numeric_limits<U16>::max() + .5F);
		m_pImpl->clearcolor.blue = static_cast<U16>(b * std::numeric_limits<U16>::max() + .5F);
		m_pImpl->clearcolor.flags = DoRed | DoGreen | DoBlue;

		XAllocColor(m_pImpl->display, DefaultColormap(m_pImpl->display, DefaultScreen(m_pImpl->display)), &m_pImpl->clearcolor);
		XSetForeground(m_pImpl->display, m_pImpl->gc, m_pImpl->clearcolor.pixel);
	}

	auto Renderer::Clear() -> void
	{
		XFillRectangle(
			m_pImpl->display,
			m_pImpl->pixmap,
			m_pImpl->gc,
			0,
			0,
			Window().Width(),
			Window().Height()
		);
	}

	auto Renderer::Render() -> void
	{
		XCopyArea(
			m_pImpl->display,
			m_pImpl->pixmap,
			m_pImpl->xwindow,
			m_pImpl->gc,
			0,
			0,
			Window().Width(),
			Window().Height(),
			0,
			0
		);
	}
}

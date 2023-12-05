#include "GFX/Platform/Software/Linux/X11/Renderer.hpp"

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#undef GLFW_EXPOSE_NATIVE_X11

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <limits>
#include <utility>

namespace Gaze::GFX::Platform::Software::Linux::X11 {
	struct Color
	{
		Display* display;
		XColor handle;

		Color() = default;
		Color(Display* dpy, F32 r, F32 g, F32 b)
			: display(dpy)
		{
			// Xlib stores colors in the 0-UINT16_MAX range, so scale as necessary
			handle.red = static_cast<U16>(r * std::numeric_limits<U16>::max() + .5F);
			handle.green = static_cast<U16>(g * std::numeric_limits<U16>::max() + .5F);
			handle.blue = static_cast<U16>(b * std::numeric_limits<U16>::max() + .5F);
			handle.flags = DoRed | DoGreen | DoBlue;

			XAllocColor(display, DefaultColormap(display, DefaultScreen(display)), &handle);
		}

		~Color()
		{
			if (display != nullptr) {
				XFreeColors(display, DefaultColormap(display, DefaultScreen(display)), &handle.pixel, 1, 0);
			}
		}

		Color(Color&& other)
			: display(std::exchange(other.display, nullptr))
			, handle(std::exchange(other.handle, XColor{}))
		{
		}

		auto operator=(Color&& other)
		{
			if (this != &other) {
				display = std::exchange(other.display, nullptr);
				handle = std::exchange(other.handle, XColor{});
			}
		}

		Color(const Color&) = delete;
		auto operator=(const Color&) -> Color& = delete;
	};

	struct Renderer::Impl
	{
		Display* display;
		XID xwindow;
		GC gc;
		Pixmap pixmap;
		Color clearColor;
		Color foregroundColor;
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

		m_pImpl->clearColor = Color(m_pImpl->display, 0.0F, 0.0F, 0.0F);
		m_pImpl->foregroundColor = Color(m_pImpl->display, 1.0F, 1.0F, 1.0F);
	}

	Renderer::~Renderer()
	{
		XFreeGC(m_pImpl->display, m_pImpl->gc);
		XFreePixmap(m_pImpl->display, m_pImpl->pixmap);
		delete m_pImpl;
	}

	auto Renderer::SetClearColor(F32 r, F32 g, F32 b, F32 /* a */) -> void
	{
		// TODO: Skipping the Color allocation if the new is the same as the old one might be a good idea.
		//   Not a big deal, though. As this function shouldn't be called that frequently anyway.
		m_pImpl->clearColor = Color(m_pImpl->display, r, g, b);
	}

	auto Renderer::Clear() -> void
	{
		XSetForeground(m_pImpl->display, m_pImpl->gc, m_pImpl->clearColor.handle.pixel);
		XFillRectangle(
			m_pImpl->display,
			m_pImpl->pixmap,
			m_pImpl->gc,
			0,
			0,
			unsigned(Window().Width()),
			unsigned(Window().Height())
		);
		XSetForeground(m_pImpl->display, m_pImpl->gc, m_pImpl->foregroundColor.handle.pixel);
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
			unsigned(Window().Width()),
			unsigned(Window().Height()),
			0,
			0
		);
	}

	auto Renderer::DrawPoint(Point2D p) -> void
	{
		XDrawPoint(
			m_pImpl->display,
			m_pImpl->pixmap,
			m_pImpl->gc,
			int(((p.x + 1) / 2) * F32(Window().Width())),
			int(((1 - p.y) / 2) * F32(Window().Height()))
		);
	}

	auto Renderer::DrawLine(Point2D start, Point2D end) -> void
	{
		XDrawLine(
			m_pImpl->display,
			m_pImpl->pixmap,
			m_pImpl->gc,
			int(((start.x + 1) / 2) * F32(Window().Width())),
			int(((1 - start.y) / 2) * F32(Window().Height())),
			int(((end.x + 1) / 2) * F32(Window().Width())),
			int(((1 - end.y) / 2) * F32(Window().Height()))
		);
	}

	auto Renderer::DrawTri(const std::array<Point2D, 3>& ps) -> void
	{
		XPoint points[] = {
			{
				short(((ps[0].x + 1) / 2) * F32(Window().Width())),
				short(((1 - ps[0].y) / 2) * F32(Window().Height())),
			},
			{
				short(((ps[1].x + 1) / 2) * F32(Window().Width())),
				short(((1 - ps[1].y) / 2) * F32(Window().Height())),
			},
			{
				short(((ps[2].x + 1) / 2) * F32(Window().Width())),
				short(((1 - ps[2].y) / 2) * F32(Window().Height())),
			},
			{
				short(((ps[0].x + 1) / 2) * F32(Window().Width())),
				short(((1 - ps[0].y) / 2) * F32(Window().Height())),
			},
		};

		XDrawLines(
			m_pImpl->display,
			m_pImpl->pixmap,
			m_pImpl->gc,
			points,
			sizeof(points) / sizeof(*points),
			CoordModeOrigin
		);
	}

	auto Renderer::FillTri(const std::array<Point2D, 3>& ps) -> void
	{
		XPoint points[] = {
			{
				short(((ps[0].x + 1) / 2) * F32(Window().Width())),
				short(((1 - ps[0].y) / 2) * F32(Window().Height())),
			},
			{
				short(((ps[1].x + 1) / 2) * F32(Window().Width())),
				short(((1 - ps[1].y) / 2) * F32(Window().Height())),
			},
			{
				short(((ps[2].x + 1) / 2) * F32(Window().Width())),
				short(((1 - ps[2].y) / 2) * F32(Window().Height())),
			},
			{
				short(((ps[0].x + 1) / 2) * F32(Window().Width())),
				short(((1 - ps[0].y) / 2) * F32(Window().Height())),
			},
		};

		XFillPolygon(
			m_pImpl->display,
			m_pImpl->pixmap,
			m_pImpl->gc,
			points,
			sizeof(points) / sizeof(*points),
			CoordModeOrigin,
			Complex // TODO: Other options can improve performance but automatically choosing them is not practical
		);
	}
}

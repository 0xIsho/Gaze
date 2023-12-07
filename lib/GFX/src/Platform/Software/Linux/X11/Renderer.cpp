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

	struct Viewport
	{
		I32 x;
		I32 y;
		I32 width;
		I32 height;
	};

	struct Renderer::Impl
	{
		Display* display;
		XID xwindow;
		GC gc;
		Pixmap pixmap;
		Viewport viewport;
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
		SetViewport(0, 0, Window().Width(), Window().Height());
		m_pImpl->pixmap = XCreatePixmap(
			m_pImpl->display,
			m_pImpl->xwindow,
			static_cast<unsigned>(m_pImpl->viewport.width),
			static_cast<unsigned>(m_pImpl->viewport.height),
			unsigned(DefaultDepth(m_pImpl->display, DefaultScreen(m_pImpl->display)))
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
		const auto& vp = m_pImpl->viewport;
		XSetForeground(m_pImpl->display, m_pImpl->gc, m_pImpl->clearColor.handle.pixel);
		XFillRectangle(
			m_pImpl->display,
			m_pImpl->pixmap,
			m_pImpl->gc,
			vp.x,
			vp.y,
			unsigned(vp.width),
			unsigned(vp.height)
		);
		XSetForeground(m_pImpl->display, m_pImpl->gc, m_pImpl->foregroundColor.handle.pixel);
	}

	auto Renderer::Render() -> void
	{
		const auto oldVp = m_pImpl->viewport;
		SetViewport(0, 0, Window().Width(), Window().Height());

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

		SetViewport(oldVp.x, oldVp.y, oldVp.width, oldVp.height);
	}

	auto Renderer::SetViewport(I32 x, I32 y, I32 width, I32 height) -> void
	{
		m_pImpl->viewport = { x, y, width, height };

		auto rect = XRectangle{
			short(x),
			short(y),
			static_cast<unsigned short>(width),
			static_cast<unsigned short>(height) };
		XSetClipRectangles(m_pImpl->display, m_pImpl->gc, 0, 0, &rect, 1, Unsorted);
	}

	auto Renderer::DrawPoint(Vec3 p) -> void
	{
		NDCtoScreen(p);

		XDrawPoint(
			m_pImpl->display,
			m_pImpl->pixmap,
			m_pImpl->gc,
			int(p.x),
			int(p.y)
		);
	}

	auto Renderer::DrawLine(Vec3 start, Vec3 end) -> void
	{
		NDCtoScreen(start);
		NDCtoScreen(end);

		XDrawLine(
			m_pImpl->display,
			m_pImpl->pixmap,
			m_pImpl->gc,
			int(start.x),
			int(start.y),
			int(end.x),
			int(end.y)
		);
	}

	auto Renderer::DrawTri(const std::array<Vec3, 3>& ps) -> void
	{
		auto psTmp = ps;
		NDCtoScreen(psTmp[0]);
		NDCtoScreen(psTmp[1]);
		NDCtoScreen(psTmp[2]);

		XPoint points[] = {
			{ short(psTmp[0].x), short(psTmp[0].y) },
			{ short(psTmp[1].x), short(psTmp[1].y) },
			{ short(psTmp[2].x), short(psTmp[2].y) },
			{ short(psTmp[0].x), short(psTmp[0].y) },
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

	auto Renderer::FillTri(const std::array<Vec3, 3>& ps) -> void
	{
		auto psTmp = ps;
		NDCtoScreen(psTmp[0]);
		NDCtoScreen(psTmp[1]);
		NDCtoScreen(psTmp[2]);

		XPoint points[] = {
			{ short(psTmp[0].x), short(psTmp[0].y) },
			{ short(psTmp[1].x), short(psTmp[1].y) },
			{ short(psTmp[2].x), short(psTmp[2].y) },
			{ short(psTmp[0].x), short(psTmp[0].y) },
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

	auto Renderer::NDCtoScreen(glm::vec3& vec) -> void
	{
		const auto& vp = m_pImpl->viewport;

		vec.x = (vec.x + 1) * (F32(vp.width) / 2) + F32(vp.x);
		vec.y = (1 - vec.y) * (F32(vp.height) / 2) + F32(vp.y);
		vec.z = (vec.z + 1) / 2;
	}
}

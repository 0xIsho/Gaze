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
		glm::mat4 projectionMat;
		Mem::Shared<Camera> camera;
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
		m_pImpl->projectionMat = glm::mat4(1.0F);
	}

	Renderer::~Renderer()
	{
		XFreeGC(m_pImpl->display, m_pImpl->gc);
		XFreePixmap(m_pImpl->display, m_pImpl->pixmap);
		delete m_pImpl;
	}

	auto Renderer::SetColor(F32 r, F32 g, F32 b, F32 /*a*/) -> void
	{
		m_pImpl->foregroundColor = Color(m_pImpl->display, r, g, b);
		XSetForeground(m_pImpl->display, m_pImpl->gc, m_pImpl->foregroundColor.handle.pixel);
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

	auto Renderer::SetProjection(glm::mat4 projection) -> void
	{
		m_pImpl->projectionMat = std::move(projection);
	}

	auto Renderer::SetCamera(Mem::Shared<Camera> camera) -> void
	{
		m_pImpl->camera = std::move(camera);
	}

	auto Renderer::DrawMesh(const Mesh& mesh, PrimitiveMode mode) -> void
	{
		auto vertices = std::vector<XPoint>();
		vertices.reserve(mesh.Vertices().size());

		{
			const auto& tmpVerts = mesh.Vertices();
			for (const auto idx : mesh.Indices()) {
				auto vert = tmpVerts[U64(idx)];
				const auto viewMat = m_pImpl->camera ? m_pImpl->camera->ComputeViewMatrix() : glm::mat4(1.0F);
				vert = m_pImpl->projectionMat * viewMat * mesh.Transform() * glm::vec4(vert, 1.0F);
				NDCtoScreen(vert);
				auto p = XPoint{ short(vert.x), short(vert.y) };
				vertices.push_back(p);
			}
		}

		switch (mode) {
		case PrimitiveMode::Points:
			XDrawPoints(
				m_pImpl->display,
				m_pImpl->pixmap,
				m_pImpl->gc,
				vertices.data(),
				int(vertices.size()),
				CoordModeOrigin
			);
			break;
		case PrimitiveMode::Lines:
			XDrawSegments(
				m_pImpl->display,
				m_pImpl->pixmap, m_pImpl->gc,
				reinterpret_cast<XSegment*>(vertices.data()),
				int(vertices.size() / 2)
			);
			break;
		case PrimitiveMode::LineLoop:
			{
				/**
				 * A LineLoop is a LineStrip with the last point connected to the first one.
				 *
				 * Here we check if the first and last vertices refer to the same one and if not,
				 * push "first" into the end of the vector to make ends meet.
				 */
				const auto first = vertices.front();
				const auto last = vertices.back();
				if ((first.x != last.x) || (first.y != last.y)) {
					vertices.push_back(first);
				}
			}
			[[fallthrough]];
		case PrimitiveMode::LineStrip:
			XDrawLines(
				m_pImpl->display,
				m_pImpl->pixmap,
				m_pImpl->gc,
				vertices.data(),
				int(vertices.size()),
				CoordModeOrigin
			);
			break;
		case PrimitiveMode::Triangles:
			// `leftovers` is either 1 or 2 extra vertices that wouldn't make a full triangle.
			// e.g. 8 vertices will make 2 triangles with 2 vertices leftover; we skip those.
			for (auto i = U64(0), leftovers = vertices.size() % 3; i < vertices.size() - leftovers; i += 3) {
				XFillPolygon(
					m_pImpl->display,
					m_pImpl->pixmap,
					m_pImpl->gc,
					vertices.data() + i,
					3,
					Complex,
					CoordModeOrigin
				);
			}
			break;
		case PrimitiveMode::TriangleStrip:
			{
				const auto& indices = mesh.Indices();
				for (auto i = U64(2); i < indices.size(); i++) {
					// Triangle strips are rendered in alternating winding order. That's what we're doing here.
					auto idx1 = indices[i - (i % 2 ? 2 : 1)];
					auto idx2 = indices[i - (i % 2 ? 1 : 2)];
					auto idx3 = indices[i];

					XPoint ps[] = { vertices[idx1], vertices[idx2], vertices[idx3] };

					XFillPolygon(
						m_pImpl->display,
						m_pImpl->pixmap,
						m_pImpl->gc,
						ps,
						3,
						Complex,
						CoordModeOrigin
					);
				}
			}
			break;
		case PrimitiveMode::TriangleFan:
			for (auto i = U64(2); i < vertices.size(); i++) {
				XPoint ps[] = { vertices[0], vertices[i - 1], vertices[i] };

				XFillPolygon(
					m_pImpl->display,
					m_pImpl->pixmap,
					m_pImpl->gc,
					ps,
					3,
					Complex,
					CoordModeOrigin
				);
			}
			break;
		}
	}

	auto Renderer::DrawPoint(Vec3 p) -> void
	{
		DrawMesh({ { std::move(p) }, { 0 } }, PrimitiveMode::Points);
	}

	auto Renderer::DrawLine(Vec3 start, Vec3 end) -> void
	{
		DrawMesh({ { std::move(start), std::move(end) }, { 0, 1 } }, PrimitiveMode::Lines);
	}

	auto Renderer::DrawTri(const std::array<Vec3, 3>& ps) -> void
	{
		DrawMesh({ { ps[0], ps[1], ps[2] }, { 0, 1, 2 } }, PrimitiveMode::LineLoop);
	}

	auto Renderer::FillTri(const std::array<Vec3, 3>& ps) -> void
	{
		DrawMesh({ { ps[0], ps[1], ps[2] }, { 0, 1, 2 } }, PrimitiveMode::Triangles);
	}

	auto Renderer::NDCtoScreen(glm::vec3& vec) -> void
	{
		const auto& vp = m_pImpl->viewport;

		vec.x = (vec.x + 1) * (F32(vp.width) / 2) + F32(vp.x);
		vec.y = (1 - vec.y) * (F32(vp.height) / 2) + F32(vp.y);
		vec.z = (vec.z + 1) / 2;
	}
}

set(TARGET GFX)

add_subdirectory("vendor/")

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

set(HEADERS
	"include/GFX/API.hpp"
	"include/GFX/Camera.hpp"
	"include/GFX/Light.hpp"
	"include/GFX/Material.hpp"
	"include/GFX/Mesh.hpp"
	"include/GFX/Object.hpp"
	"include/GFX/Primitives.hpp"
	"include/GFX/Renderer.hpp"

	"include/GFX/Platform/OpenGL/Renderer.hpp"

	"include/GFX/Platform/OpenGL/Objects/Object.hpp"
	"include/GFX/Platform/OpenGL/Objects/IndexBuffer.hpp"
	"include/GFX/Platform/OpenGL/Objects/Shader.hpp"
	"include/GFX/Platform/OpenGL/Objects/VertexArray.hpp"
	"include/GFX/Platform/OpenGL/Objects/VertexBuffer.hpp"
)

set(SOURCES
	"src/API.cpp"
	"src/Camera.cpp"
	"src/Mesh.cpp"
	"src/Object.cpp"
	"src/Primitives.cpp"
	"src/Renderer.cpp"

	"src/Platform/OpenGL/Renderer.cpp"
	"src/Platform/OpenGL/Objects/IndexBuffer.cpp"
	"src/Platform/OpenGL/Objects/Shader.cpp"
	"src/Platform/OpenGL/Objects/VertexArray.cpp"
	"src/Platform/OpenGL/Objects/VertexBuffer.cpp"
)

add_library(${TARGET} ${HEADERS} ${SOURCES})
add_library(Gaze::${TARGET} ALIAS ${TARGET})
append_common_compiler_options(${TARGET})

target_include_directories(${TARGET}
	PUBLIC
		"include/"
)

target_link_libraries(${TARGET}
	PUBLIC
		Gaze::Geometry
		Gaze::Core
		Gaze::WM

		glm::glm

	PRIVATE
		Gaze::Debug
		Gaze::GLAD
		Gaze::Log

		OpenGL::GL
		glfw
)

target_precompile_headers(${TARGET}
	PRIVATE
		"include/GFX/pch.hpp"
)

if (GAZE_BUILD_TESTS)
	add_subdirectory("tests/")
endif()

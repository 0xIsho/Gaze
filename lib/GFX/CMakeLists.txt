set(TARGET GFX)

add_subdirectory("vendor/")

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

if(UNIX)
	find_package(X11 REQUIRED)
else()
	message(FATAL_ERROR "The renderer does not support the target platform: ${CMAKE_HOST_SYSTEM_NAME}")
endif()

set(HEADERS
	"include/GFX/API.hpp"
	"include/GFX/Camera.hpp"
	"include/GFX/Mesh.hpp"
	"include/GFX/Renderer.hpp"

	"include/GFX/Platform/OpenGL/Renderer.hpp"
	"include/GFX/Platform/Software/Renderer.hpp"

	$<$<BOOL:UNIX>:
		"include/GFX/Platform/Software/Linux/Renderer.hpp"
		"include/GFX/Platform/Software/Linux/X11/Renderer.hpp"
	>
)

set(SOURCES
	"src/API.cpp"
	"src/Camera.cpp"
	"src/Mesh.cpp"
	"src/Renderer.cpp"

	"src/Platform/OpenGL/Renderer.cpp"

	$<$<BOOL:UNIX>:
		"src/Platform/Software/Linux/X11/Renderer.cpp"
	>
)

add_library(${TARGET} ${HEADERS} ${SOURCES})
add_library(Gaze::${TARGET} ALIAS ${TARGET})
append_common_compiler_options(${TARGET})

target_include_directories(${TARGET}
	PUBLIC
		"include/"

		${GLM_INCLUDE_DIRS}

	PRIVATE
		$<$<BOOL:UNIX>:
			${X11_INCLUDE_DIR}
		>
)

target_link_libraries(${TARGET}
	PUBLIC
		Gaze::Debug
		Gaze::Core
		Gaze::WM

	PRIVATE
		OpenGL::GL
		Gaze::GLAD
		glfw

		$<$<BOOL:UNIX>:
			${X11_LIBRARIES}
		>
)

if (GAZE_BUILD_TESTS)
	add_subdirectory("tests/")
endif()

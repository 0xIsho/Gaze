set(TARGET GFX)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

set(HEADERS
	"include/GFX/API.hpp"
	"include/GFX/Renderer.hpp"

	"include/GFX/Platform/Software/Renderer.hpp"

	$<$<BOOL:UNIX>:
		"include/GFX/Platform/Software/Linux/Renderer.hpp"
		"include/GFX/Platform/Software/Linux/X11/Renderer.hpp"
	>
)

set(SOURCES
	"src/API.cpp"
	"src/Renderer.cpp"

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
)

target_link_libraries(${TARGET}
	PUBLIC
		Gaze::Core
		Gaze::WM

	PRIVATE
		OpenGL::GL
		glfw
)

if (GAZE_BUILD_TESTS)
	add_subdirectory("tests/")
endif()

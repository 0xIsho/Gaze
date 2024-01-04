set(TARGET WM)

find_package(glfw3 REQUIRED)

set(HEADERS
	"include/WM/Core.hpp"
	"include/WM/Window.hpp"
)

set(SOURCES
	"src/Core.cpp"
	"src/Window.cpp"
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
		Gaze::Events
	PRIVATE
		Gaze::Debug

		glfw
)

target_precompile_headers(${TARGET}
	PRIVATE
		"include/WM/pch.hpp"
)

if (GAZE_BUILD_TESTS)
	add_subdirectory("tests/")
endif()

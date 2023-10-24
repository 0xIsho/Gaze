set(TARGET GFX)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

set(HEADERS

)

set(SOURCES
	"src/dummy.cpp"
)

add_library(${TARGET} ${HEADERS} ${SOURCES})
add_library(Gaze::${TARGET} ALIAS ${TARGET})
append_common_compiler_options(${TARGET})

target_include_directories(${TARGET}
	PUBLIC
		"include/"
)

target_link_libraries(${TARGET}
	PRIVATE
		OpenGL::GL
		glfw
)

if (GAZE_BUILD_TESTS)
	add_subdirectory("tests/")
endif()

set(TARGET Geometry)

set(HEADERS
	"include/Geometry/Mesh.hpp"
)

set(SOURCES
	"src/Mesh.cpp"
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
)

if (GAZE_BUILD_TESTS)
	add_subdirectory("tests/")
endif()

set(TARGET Debug)

set(HEADERS
	"include/Debug/Assert.hpp"
)

set(SOURCES
	"src/Assert.cpp"
)

add_library(${TARGET} ${SOURCES} ${HEADERS})
add_library(Gaze::${TARGET} ALIAS ${TARGET})
append_common_compiler_options(${TARGET})

target_include_directories(${TARGET}
	PUBLIC
		"include/"
)

if (GAZE_BUILD_TESTS)
	add_subdirectory("tests/")
endif()

set(TARGET Client)

set(HEADERS
)

set(SOURCES
	"src/dummy.cpp"
)

add_library(${TARGET} ${HEADERS} ${SOURCES})
append_common_compiler_options(${TARGET})

if (GAZE_BUILD_TESTS)
	add_subdirectory("tests/")
endif()

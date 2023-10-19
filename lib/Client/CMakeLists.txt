set(TARGET Client)

set(HEADERS
	"include/Client/App.hpp"
)

set(SOURCES
	"src/App.cpp"
)

add_library(${TARGET} ${HEADERS} ${SOURCES})
add_library(Gaze::${TARGET} ALIAS ${TARGET})
append_common_compiler_options(${TARGET})

target_include_directories(${TARGET}
	PUBLIC
		"include/"
)

if (GAZE_BUILD_TESTS)
	add_subdirectory("tests/")
endif()
set(TARGET Config)

set(HEADERS
	"include/Config/Configuration.hpp"
	"include/Config/Parser.hpp"
)

set(SOURCES
	"src/Configuration.cpp"
	"src/Parser.cpp"
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
		Gaze::Debug
)

if (GAZE_BUILD_TESTS)
	add_subdirectory("tests/")
endif()

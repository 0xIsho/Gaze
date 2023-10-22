set(TARGET Core)

set(HEADERS
	"include/Core/Platform.hpp"
	"include/Core/Type.hpp"
)

set(SOURCES
)

add_library(${TARGET} INTERFACE ${HEADERS} ${SOURCES})
add_library(Gaze::${TARGET} ALIAS ${TARGET})
# append_common_compiler_options(${TARGET})

target_include_directories(${TARGET}
	INTERFACE
		"include/"
)

if (GAZE_BUILD_TESTS)
	add_subdirectory("tests/")
endif()

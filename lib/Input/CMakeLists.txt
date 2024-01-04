set(TARGET Input)

set(SOURCES
)

set(HEADERS
	"include/Input/KeyCode.hpp"
)

add_library(${TARGET} INTERFACE "${SOURCES}" "${HEADERS}")
add_library(Gaze::${TARGET} ALIAS ${TARGET})
# append_common_compiler_options(${TARGET})

# TODO: Handle include path on install
target_include_directories(${TARGET}
	INTERFACE
		"include/"
)

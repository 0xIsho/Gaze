set(TARGET Input)

set(SOURCES
	"src/Input.cpp"
)

set(HEADERS
	"include/Input/Input.hpp"
	"include/Input/KeyCode.hpp"
)

add_library(${TARGET} "${SOURCES}" "${HEADERS}")
add_library(Gaze::${TARGET} ALIAS ${TARGET})
append_common_compiler_options(${TARGET})

# TODO: Handle include path on install
target_include_directories(${TARGET}
	PUBLIC
		"include/"
)

target_link_libraries(${TARGET}
	PUBLIC
		Gaze::Core
		Gaze::WM

	PRIVATE
		glfw
)

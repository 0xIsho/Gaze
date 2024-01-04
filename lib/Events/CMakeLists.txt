set(TARGET Events)

set(HEADERS
	"include/Events/Dispatcher.hpp"
	"include/Events/Event.hpp"
	"include/Events/KeyEvent.hpp"
	"include/Events/MouseEvent.hpp"
	"include/Events/WindowEvent.hpp"
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

target_link_libraries(${TARGET}
	INTERFACE
		Gaze::Input
)

if (GAZE_BUILD_TESTS)
	add_subdirectory("tests/")
endif()

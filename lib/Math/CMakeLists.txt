set(TARGET Math)

set(SOURCES
	"include/Math/Util.hpp"
	"include/Math/Vector.hpp"
)

add_library(${TARGET} INTERFACE ${SOURCES})
add_library(Gaze::${TARGET} ALIAS ${TARGET})

target_include_directories(
	${TARGET}
		INTERFACE
			"include/"
)

target_link_libraries(
	${TARGET}
		INTERFACE
			Gaze::Core
			Gaze::Debug
)

if (GAZE_BUILD_TESTS)
	add_subdirectory(test)
endif()

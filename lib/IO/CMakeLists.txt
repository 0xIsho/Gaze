set(TARGET IO)

find_package(assimp REQUIRED)

set(HEADERS
	"include/IO/Loader/Scene.hpp"
)

set(SOURCES
	"src/Loader/Scene.cpp"
)

add_library(${TARGET} ${HEADERS} ${SOURCES})
add_library(Gaze::${TARGET} ALIAS ${TARGET})
append_common_compiler_options(${TARGET})

target_include_directories(${TARGET}
	PUBLIC
		"include/"

	PRIVATE
		${ASSIMP_INCLUDE_DIRS}
)

target_link_libraries(${TARGET}
	PUBLIC
		Gaze::Geometry

	PRIVATE
		${ASSIMP_LIBRARIES}
)

if (GAZE_BUILD_TESTS)
	add_subdirectory("tests/")
endif()

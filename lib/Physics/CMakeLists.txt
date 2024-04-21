set(TARGET Physics)

find_package(Bullet REQUIRED)
find_package(glm REQUIRED)

set(HEADERS
	"include/Physics/Rigidbody.hpp"
	"include/Physics/Shape.hpp"
	"include/Physics/World.hpp"
)

set(SOURCES
	"src/Rigidbody.cpp"
	"src/Shape.cpp"
	"src/World.cpp"
)

add_library(${TARGET} ${HEADERS} ${SOURCES})
add_library(Gaze::${TARGET} ALIAS ${TARGET})
append_common_compiler_options(${TARGET})

target_include_directories(${TARGET}
	PUBLIC
		"include/"
)

target_include_directories(${TARGET}
	SYSTEM PRIVATE # SYSTEM so that the compiler doesn't issue warnings (there are lots of them)
		${BULLET_INCLUDE_DIRS}
)

target_link_libraries(${TARGET}
	PUBLIC
		glm::glm

	PUBLIC
		Gaze::Core

		${BULLET_LIBRARIES}
)

if (GAZE_BUILD_TESTS)
	add_subdirectory("tests/")
endif()

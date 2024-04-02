set(TARGET Log)

find_package(spdlog REQUIRED)

set(HEADERS
	"include/Log/Logger.hpp"
)

set(SOURCES
	"src/Logger.cpp"
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
		spdlog::spdlog
)

if (GAZE_BUILD_TESTS)
	add_subdirectory("tests/")
endif()

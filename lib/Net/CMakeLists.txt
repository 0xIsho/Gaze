set(TARGET Net)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ENet REQUIRED libenet)

set(HEADERS
	"include/Net/Core.hpp"
)

set(SOURCES
	"src/Core.cpp"
)

add_library(${TARGET} ${HEADERS} ${SOURCES})
add_library(Gaze::${TARGET} ALIAS ${TARGET})
append_common_compiler_options(${TARGET})

target_include_directories(${TARGET}
	PUBLIC
		"include/"

	PRIVATE
		${ENet_INCLUDE_DIRS}
)

target_link_libraries(${TARGET}
	PRIVATE
		Gaze::Debug

		${ENet_LIBRARIES}
)

target_compile_options(${TARGET}
	PRIVATE
		${ENet_CFLAGS_OTHER}
)

if (GAZE_BUILD_TESTS)
	add_subdirectory("tests/")
endif()

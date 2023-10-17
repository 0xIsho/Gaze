set(TARGET Sandbox)

set(SOURCES
	"src/main.cpp"
)

add_executable(${TARGET} ${SOURCES})
append_common_compiler_options(${TARGET})

if (GAZE_BUILD_TESTS)
	add_subdirectory("tests/")
endif()

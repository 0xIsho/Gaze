set(TARGET Sandbox)

set(SOURCES
	"src/main.cpp"
)

add_executable(${TARGET} ${SOURCES})
append_common_compiler_options(${TARGET})

target_link_libraries(${TARGET}
	PRIVATE
		Gaze::Client
		Gaze::WM
)

if (GAZE_BUILD_TESTS)
	add_subdirectory("tests/")
endif()

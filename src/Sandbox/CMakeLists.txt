set(CLIENT_TARGET SandboxClient)
set(SERVER_TARGET SandboxServer)

add_executable(${CLIENT_TARGET} "src/Client.cpp")
append_common_compiler_options(${CLIENT_TARGET})
target_link_libraries(${CLIENT_TARGET}
	PRIVATE
		Gaze::Client
		Gaze::Input
		Gaze::GFX
		Gaze::WM
)

add_executable(${SERVER_TARGET} "src/Server.cpp")
append_common_compiler_options(${SERVER_TARGET})
target_link_libraries(${SERVER_TARGET}
	PRIVATE
		Gaze::Client
)

if (GAZE_BUILD_TESTS)
	add_subdirectory("tests/")
endif()

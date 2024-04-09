set(CLIENT_TARGET BrickPopperClient)
set(SERVER_TARGET BrickPopperServer)

find_package(glm REQUIRED)

add_executable(${CLIENT_TARGET} "Client.cpp")

if (WIN32)
set_target_properties(${CLIENT_TARGET} PROPERTIES WIN32_EXECUTABLE ON)
target_link_options(${CLIENT_TARGET} PRIVATE "/ENTRY:mainCRTStartup")
endif()

append_common_compiler_options(${CLIENT_TARGET})
target_link_libraries(${CLIENT_TARGET}
	PRIVATE
		Gaze::Client
		Gaze::Events
		Gaze::Input
		Gaze::Core
		Gaze::GFX
		Gaze::WM
)

add_executable(${SERVER_TARGET} "Server.cpp")
append_common_compiler_options(${SERVER_TARGET})
target_link_libraries(${SERVER_TARGET}
	PRIVATE
		Gaze::Client
)

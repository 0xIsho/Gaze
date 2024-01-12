set(CLIENT_TARGET PongClient)
set(SERVER_TARGET PongServer)

find_package(glm REQUIRED)

add_executable(${CLIENT_TARGET} "Client.cpp" "Type.hpp")
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

add_executable(${SERVER_TARGET} "Server.cpp" "Type.hpp")
append_common_compiler_options(${SERVER_TARGET})
target_link_libraries(${SERVER_TARGET}
	PRIVATE
		Gaze::Client
		Gaze::Net
)
target_include_directories(${SERVER_TARGET}
	PRIVATE
		${GLM_INCLUDE_DIRS}
)

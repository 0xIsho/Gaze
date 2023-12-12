set(TARGET Pong)

add_executable(${TARGET} "main.cpp")
append_common_compiler_options(${TARGET})
target_link_libraries(${TARGET}
	PRIVATE
		Gaze::Client
		Gaze::Input
		Gaze::Core
		Gaze::GFX
		Gaze::WM
)

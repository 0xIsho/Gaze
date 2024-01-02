set(TARGET HelloInput)

add_executable(${TARGET} "main.cpp")
target_link_libraries(${TARGET}
	PRIVATE
		Gaze::Client
		Gaze::Events
		Gaze::Core
		Gaze::WM
		Gaze::Input
)

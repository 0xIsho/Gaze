set(TARGET HelloViewport)

add_executable(${TARGET} "main.cpp")
target_link_libraries(${TARGET}
	PRIVATE
		Gaze::Client
		Gaze::Events
		Gaze::Core
		Gaze::GFX
		Gaze::WM
)

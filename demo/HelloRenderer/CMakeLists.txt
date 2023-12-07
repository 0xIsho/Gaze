set(TARGET HelloRenderer)

add_executable(${TARGET} "main.cpp")
target_link_libraries(${TARGET}
	PRIVATE
		Gaze::Client
		Gaze::Core
		Gaze::GFX
		Gaze::WM
)

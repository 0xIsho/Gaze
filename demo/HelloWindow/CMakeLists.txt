set(TARGET HelloWindow)

add_executable(${TARGET} "main.cpp")
target_link_libraries(${TARGET}
	PRIVATE
		Gaze::Client
		Gaze::WM
)

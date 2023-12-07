set(TARGET HelloWorld)

add_executable(${TARGET} "main.cpp")
target_link_libraries(${TARGET}
	PRIVATE
		Gaze::Client
)

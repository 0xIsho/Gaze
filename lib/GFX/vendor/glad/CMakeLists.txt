add_library(glad "src/gl.c")
add_library(Gaze::GLAD ALIAS glad)
target_include_directories(glad
	PUBLIC
		"include/"
)

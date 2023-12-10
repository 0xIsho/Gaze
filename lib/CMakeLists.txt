set(LIBS
	Client
	Core
	Debug
	GFX
	Input
	WM
)

foreach(LIB ${LIBS})
	add_subdirectory("${LIB}")
endforeach()

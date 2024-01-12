set(LIBS
	Client
	Core
	Debug
	Events
	GFX
	Input
	Net
	WM
)

foreach(LIB ${LIBS})
	add_subdirectory("${LIB}")
endforeach()

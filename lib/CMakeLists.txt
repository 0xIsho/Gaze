set(LIBS
	Client
	Core
	Debug
	Events
	GFX
	Input
	IO
	Net
	Physics
	WM
)

foreach(LIB ${LIBS})
	add_subdirectory("${LIB}")
endforeach()

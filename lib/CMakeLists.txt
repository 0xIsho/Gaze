set(LIBS
	Client
	Core
	Debug
	Events
	GFX
	Input
	WM
)

foreach(LIB ${LIBS})
	add_subdirectory("${LIB}")
endforeach()

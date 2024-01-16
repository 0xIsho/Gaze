set(LIBS
	Client
	Core
	Debug
	Events
	Geometry
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

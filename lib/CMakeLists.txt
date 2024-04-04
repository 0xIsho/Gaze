set(LIBS
	Client
	Config
	Core
	Debug
	Events
	Geometry
	GFX
	Input
	IO
	Log
	Net
	Physics
	WM
)

foreach(LIB ${LIBS})
	add_subdirectory("${LIB}")
endforeach()

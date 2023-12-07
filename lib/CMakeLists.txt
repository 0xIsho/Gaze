set(LIBS
	Client
	Core
	Debug
	GFX
	Input
	Test
	WM
)

foreach(LIB ${LIBS})
	add_subdirectory("${LIB}")
endforeach()

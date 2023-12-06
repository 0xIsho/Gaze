set(LIBS
	Client
	Core
	Debug
	GFX
	Test
	WM
)

foreach(LIB ${LIBS})
	add_subdirectory("${LIB}")
endforeach()

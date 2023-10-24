set(LIBS
	Client
	Core
	Debug
	GFX
	Math
	Test
	WM
)

foreach(LIB ${LIBS})
	add_subdirectory("${LIB}")
endforeach()

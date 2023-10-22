set(LIBS
	Client
	Core
	Debug
	Math
	Test
	WM
)

foreach(LIB ${LIBS})
	add_subdirectory("${LIB}")
endforeach()

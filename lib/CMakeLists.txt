set(LIBS
	Client
	Core
	Debug
	Test
	WM
)

foreach(LIB ${LIBS})
	add_subdirectory("${LIB}")
endforeach()

set(LIBS
	# Client
	# Core
	# Debug
	Test
)

foreach(LIB ${LIBS})
	add_subdirectory("${LIB}")
endforeach()

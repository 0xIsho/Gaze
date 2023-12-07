set(DEMOS
	HelloWindow
	HelloWorld
)

foreach(DEMO ${DEMOS})
	add_subdirectory("${DEMO}")
endforeach()

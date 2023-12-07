set(DEMOS
	HelloWorld
)

foreach(DEMO ${DEMOS})
	add_subdirectory("${DEMO}")
endforeach()

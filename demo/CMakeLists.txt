set(DEMOS
	HelloInput
	HelloRenderer
	HelloWindow
	HelloWorld
)

foreach(DEMO ${DEMOS})
	add_subdirectory("${DEMO}")
endforeach()

set(DEMOS
	Games
	HelloInput
	HelloRenderer
	HelloViewport
	HelloWindow
	HelloWorld
)

foreach(DEMO ${DEMOS})
	add_subdirectory("${DEMO}")
endforeach()

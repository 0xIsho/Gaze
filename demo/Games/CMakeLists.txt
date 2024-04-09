set(GAMES
	BrickPopper
	Pong
)

foreach(GAME ${GAMES})
	add_subdirectory("${GAME}")
endforeach()

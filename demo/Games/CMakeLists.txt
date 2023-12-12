set(GAMES
	Pong
)

foreach(GAME ${GAMES})
	add_subdirectory("${GAME}")
endforeach()

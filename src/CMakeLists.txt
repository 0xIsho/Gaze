set(MODULES
	Sandbox
)

foreach(MOD ${MODULES})
	add_subdirectory("${MOD}")
endforeach()

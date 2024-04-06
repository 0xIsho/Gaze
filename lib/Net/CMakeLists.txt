set(TARGET Net)

add_subdirectory("vendor/enet")

set(HEADERS
	"include/Net/Client.hpp"
	"include/Net/Core.hpp"
	"include/Net/Packet.hpp"
	"include/Net/Server.hpp"
	"include/Net/Type.hpp"
)

set(SOURCES
	"src/Client.cpp"
	"src/Core.cpp"
	"src/Packet.cpp"
	"src/Server.cpp"
)

add_library(${TARGET} ${HEADERS} ${SOURCES})
add_library(Gaze::${TARGET} ALIAS ${TARGET})
append_common_compiler_options(${TARGET})

target_include_directories(${TARGET}
	PUBLIC
		"include/"
)

target_link_libraries(${TARGET}
	PUBLIC
		Gaze::Core

	PRIVATE
		Gaze::Debug
		Gaze::Log

		enet
		$<$<PLATFORM_ID:Windows>:wsock32;ws2_32;Winmm>
)


if (GAZE_BUILD_TESTS)
	add_subdirectory("tests/")
endif()

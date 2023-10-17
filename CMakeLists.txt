cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

set(GAZE_VERSION_MAJOR 0)
set(GAZE_VERSION_MINOR 1)
set(GAZE_VERSION_PATCH 0)
set(GAZE_VERSION_TWEAK 0)
set(
	GAZE_VERSION
	"${GAZE_VERSION_MAJOR}.${GAZE_VERSION_MINOR}.${GAZE_VERSION_PATCH}.${GAZE_VERSION_TWEAK}"
)

message(STATUS "Gaze Engine version: ${GAZE_VERSION}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")
include(CompilerUtils)

project(
	Gaze
	VERSION "${GAZE_VERSION}"
	DESCRIPTION "Gaze Interactive Experiences Engine"
)

option(GAZE_BUILD_TESTS "Build the test suite?" ON)

if (GAZE_BUILD_TESTS)
	find_package(Catch2 REQUIRED)
	include(Catch)
	enable_testing()
endif()

add_subdirectory("lib/")
add_subdirectory("src/")

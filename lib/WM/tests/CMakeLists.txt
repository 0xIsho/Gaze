set(TESTS
	Core
)

foreach(TEST ${TESTS})
	set(TEST_TARGET test_${TARGET}_${TEST})
	add_executable(${TEST_TARGET} ${TEST}.cpp)
	target_link_libraries(${TEST_TARGET} ${GAZE_CATCH2_TARGET} Gaze::WM)
	catch_discover_tests(${TEST_TARGET})
endforeach()

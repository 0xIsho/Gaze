set(TESTS
	dummy
)

foreach(TEST ${TESTS})
	set(TEST_TARGET test_${TARGET}_${TEST})
	add_executable(${TEST_TARGET} ${TEST}.cpp)
	target_link_libraries(${TEST_TARGET} ${GAZE_CATCH2_TARGET})
	catch_discover_tests(${TEST_TARGET})
endforeach()

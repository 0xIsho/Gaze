set(TARGET Test)

add_library(${TARGET} "main.cpp")
add_library(Gaze::${TARGET} ALIAS ${TARGET})
target_link_libraries(${TARGET} Catch2::Catch2)

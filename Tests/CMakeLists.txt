include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/tests.cmake)
skyrocket_tests_init()

add_subdirectory(Static)

add_executable(Playground Playground.cpp)
target_link_libraries(Playground Skyrocket)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/tests.cmake)
skyrocket_tests_init()

add_subdirectory(Static)

if (SKYROCKET_EXPERIMENTAL)
    add_subdirectory(Experimental)
endif ()
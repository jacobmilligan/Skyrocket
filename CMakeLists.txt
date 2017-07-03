####################################
## Skyrocket setup commands
####################################
cmake_minimum_required(VERSION 3.5)
project("Skyrocket")

set(CMAKE_CXX_STANDARD 14)

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/skyrocket.cmake)

####################################
## Definitions
####################################

set(SKYROCKET_GRAPHICS_API Metal)

####################################
## Linking
####################################

add_subdirectory(Source)
add_subdirectory(Examples/Graphics)

add_library(${PROJECT_NAME} STATIC
        ${SkyrocketFramework_sources})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/Source)

target_link_libraries(${PROJECT_NAME}
        SkyrocketCore
        SkyrocketPlatform)


#############################
## Skyrocket setup commands
#############################
cmake_minimum_required(VERSION 3.5)
project("Skyrocket")

set(CMAKE_CXX_STANDARD 14)

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/skyrocket.cmake)

################################
## Definitions and config vars
################################

add_definitions(-DSKYROCKET_GRAPHICS_API=${SKYROCKET_GRAPHICS_API})

set(SKYROCKET_BUILD_TYPE)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(SKYROCKET_BUILD_TYPE Debug)
    add_definitions(-DSKYROCKET_DEBUG)
else()
    set(SKYROCKET_BUILD_TYPE Release)
    add_definitions(-DSKYROCKET_RELEASE)
endif ()
MESSAGE("-- Skyrocket.Framework: Building in ${SKYROCKET_BUILD_TYPE} mode")

###########################
## Project subdirectories
###########################

add_subdirectory(Source)
add_subdirectory(Examples/Graphics)

MESSAGE("-- Skyrocket.Graphics: Using the ${SKYROCKET_GRAPHICS_API} API")

###########
## Linking
###########

add_library(${PROJECT_NAME} STATIC
        ${SkyrocketFramework_sources})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/Source)

target_link_libraries(${PROJECT_NAME}
        SkyrocketCore
        SkyrocketPlatform)


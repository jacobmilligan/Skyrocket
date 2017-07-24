#############################
## Skyrocket setup commands
#############################
cmake_minimum_required(VERSION 3.5)
project("Skyrocket")

set(CMAKE_CXX_STANDARD 14)

set(skyrocket_root_directory ${CMAKE_CURRENT_SOURCE_DIR})

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/skyrocket.cmake)

skyrocket_init()

################################
## Definitions and config vars
################################

MESSAGE("-- Skyrocket.Framework: Building in ${skyrocket_build_type} mode")

if (NOT DEFINED SKY_RESOURCE_DIRECTORY)
    set(SKY_RESOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif ()

ADD_DEFINITIONS(-DSKY_RESOURCE_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}/${SKY_RESOURCE_DIRECTORY}")

###########################
## Project subdirectories
###########################

add_subdirectory(Source)
add_subdirectory(Examples/Graphics)

MESSAGE("-- Skyrocket.Graphics: Using the ${skyrocket_graphics_api} API")

skyrocket_include_all_definitions()

###########
## Linking
###########

add_library(${PROJECT_NAME} STATIC
        ${SkyrocketFramework_sources})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/Source)

target_link_libraries(${PROJECT_NAME} ${skyrocket_libraries})

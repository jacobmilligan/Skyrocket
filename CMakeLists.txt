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

###########################
## Project subdirectories
###########################

add_subdirectory(Source)
add_subdirectory(Examples)
add_subdirectory(Tests)

skyrocket_include_all_definitions()

###########
## Linking
###########

add_library(${PROJECT_NAME} SHARED)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/Source)

target_link_libraries(${PROJECT_NAME} ${skyrocket_libraries})

# Dependencies
add_subdirectory(Deps/Jobrocket)
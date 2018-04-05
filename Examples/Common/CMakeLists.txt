set(CMAKE_CXX_STANDARD 14)
project(Common)

file(GLOB_RECURSE shader_sources "${CMAKE_CURRENT_SOURCE_DIR}/../Resources/Shaders/Build/*.cpp")

add_library(${PROJECT_NAME} STATIC Common.cpp ${shader_sources})
target_link_libraries(${PROJECT_NAME} Skyrocket)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../Resources/Shaders/Build)
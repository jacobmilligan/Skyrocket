set(CMAKE_CXX_STANDARD 14)
project(Common)

add_library(${PROJECT_NAME} STATIC Common.cpp)
target_link_libraries(${PROJECT_NAME} Skyrocket)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
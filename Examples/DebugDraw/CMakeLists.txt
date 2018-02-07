set(CMAKE_CXX_STANDARD 14)
project(DebugDrawExample)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} Skyrocket Common)
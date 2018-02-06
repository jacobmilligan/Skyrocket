set(CMAKE_CXX_STANDARD 14)

set(exe_name DebugDrawExample)

add_executable(${exe_name} main.cpp)
target_link_libraries(${exe_name} Skyrocket)
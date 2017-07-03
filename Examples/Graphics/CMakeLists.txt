project("Skyrocket Graphics Example")

set(exe_name skyrocket_graphics_example)

set(SRCS main.cpp)

add_executable(${exe_name} "${SRCS}")
target_link_libraries(${exe_name} Skyrocket)
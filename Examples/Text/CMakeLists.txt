set(CMAKE_CXX_STANDARD 14)

set(exe_name TextExample)

set(SRCS main.cpp)

add_executable(${exe_name} "${SRCS}")
target_link_libraries(${exe_name} Skyrocket)
target_include_directories(${exe_name} PRIVATE ${PROJECT_SOURCE_DIR}/Source)
skyrocket_include_all_definitions()

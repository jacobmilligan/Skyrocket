set(CMAKE_CXX_STANDARD 14)
project(CubesExample)

set(SRCS main.cpp)

add_executable(${PROJECT_NAME} "${SRCS}")
target_link_libraries(${PROJECT_NAME} Skyrocket Common)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/Source)
skyrocket_include_all_definitions()

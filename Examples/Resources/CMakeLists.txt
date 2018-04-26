file(GLOB shaders "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Source/*.glsl")

add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Build
        COMMAND shadecc -i ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Source -o ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Build
        DEPENDS ${shaders})

add_custom_target(
        BuildShaders ALL
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Build
)
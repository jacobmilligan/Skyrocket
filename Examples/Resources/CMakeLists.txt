file(GLOB shaders "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Source/*.glsl")

add_custom_target(
        BuildShaders
        COMMAND python3 ../../../Deps/Shadecc/shadecc/__init__.py -i Source -o Build
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Shaders
)
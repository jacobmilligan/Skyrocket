set(lib_name SkyrocketFramework)

skyrocket_add_sources(Application.cpp
        Camera.cpp)

set(dependencies SkyrocketGraphics SkyrocketPlatform Jobrocket)
skyrocket_add_library(${lib_name} STATIC)
target_link_libraries(${lib_name} "${dependencies}")
target_include_directories(${lib_name} PUBLIC ${PROJECT_SOURCE_DIR}/Deps/Jobrocket/Source)

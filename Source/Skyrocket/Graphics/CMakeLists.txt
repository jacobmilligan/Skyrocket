set(CMAKE_CXX_STANDARD 14)

set(lib_name SkyrocketGraphics)

###########################################
## Add dependencies from Skyrocket
###########################################

###########################################
## Platform-specific configurations
###########################################

skyrocket_configure_graphics_api(graphics_api)
skyrocket_configure_platform(compile_flags)
list(APPEND dependencies "${graphics_api}")

######################################
## Add subdirectories and sources
######################################

add_subdirectory(Internal)
add_subdirectory(API)
add_subdirectory(GraphicsInterface)
add_subdirectory(GraphicsCore)

######################################
## Add library and link dependencies
######################################

skyrocket_add_library(${lib_name} STATIC)
target_link_libraries(${lib_name}
        SkyrocketCore
        SkyrocketPlatform)

target_link_libraries(${lib_name} "${dependencies}")
target_compile_options(${lib_name} PRIVATE "${compile_flags}")

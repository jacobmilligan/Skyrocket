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

if (APPLE)
    # Add library dependencies
    FIND_LIBRARY(cocoa_lib Cocoa)
    if (NOT cocoa_lib)
        MESSAGE("-- Skyrocket.Platform: Could not find Cocoa framework")
    else()
        list(APPEND dependencies ${cocoa_lib})
    endif ()

    FIND_LIBRARY(quartzcore_lib QuartzCore)
    if (NOT quartzcore_lib)
        MESSAGE("-- Skyrocket.Platform: Could not find QuartzCore framework")
    else()
        list(APPEND dependencies ${quartzcore_lib})
    endif ()
endif ()

######################################
## Add subdirectories and sources
######################################

add_subdirectory(Internal)
add_subdirectory(Interface)
add_subdirectory(Core)

######################################
## Add library and link dependencies
######################################

skyrocket_add_library(${lib_name} STATIC)
target_link_libraries(${lib_name}
        SkyrocketCore
        SkyrocketPlatform)

target_link_libraries(${lib_name} "${dependencies}")
target_compile_options(${lib_name} PRIVATE "${compile_flags}")

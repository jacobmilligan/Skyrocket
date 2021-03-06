set(CMAKE_CXX_STANDARD 14)

set(lib_name SkyrocketGraphics)

###########################################
## Add dependencies from Skyrocket
###########################################

###########################################
## Platform-specific configurations
###########################################

skyrocket_configure_graphics_api(graphics_api)
list(APPEND dependencies "${graphics_api}")

foreach(api ${skyrocket_graphics_api})
    MESSAGE("-- Skyrocket.Graphics: ${api} support detected")
endforeach()

if (APPLE)
    # Add library dependencies
    FIND_LIBRARY(cocoa_lib Cocoa)
    if (NOT cocoa_lib)
        MESSAGE("-- Skyrocket.Graphics: Could not find Cocoa framework")
    else ()
        list(APPEND dependencies ${cocoa_lib})
    endif ()

    FIND_LIBRARY(quartzcore_lib QuartzCore)
    if (NOT quartzcore_lib)
        MESSAGE("-- Skyrocket.Graphics: Could not find QuartzCore framework")
    else ()
        list(APPEND dependencies ${quartzcore_lib})
    endif ()
endif ()

######################################
## Subdirectories
######################################

add_subdirectory(Renderer)

if (APPLE)
    add_subdirectory(Apple)
elseif (WIN32)
    add_subdirectory(Windows)
endif ()

######################################
## Sources
######################################

skyrocket_add_sources(Color.cpp
        Viewport.cpp
        Image.cpp)

######################################
## Add library and link dependencies
######################################

skyrocket_add_library(${lib_name} STATIC)

# Add internal deps
target_link_libraries(${lib_name}
        SkyrocketCore
        SkyrocketPlatform)

target_link_libraries(${lib_name} "${dependencies}")

target_include_directories(${lib_name} PUBLIC ${skyrocket_root_directory}/Deps/stb_image)



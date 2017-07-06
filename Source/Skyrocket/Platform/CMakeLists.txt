set(CMAKE_CXX_STANDARD 14)

set(lib_name SkyrocketPlatform)

skyrocket_add_sources(Generic/ChronoTime.cpp)

set(compile_flags)
set(dependencies)

###########################################
## Add dependencies from Skyrocket
###########################################

list(APPEND dependencies SkyrocketCore)

###########################################
## Platform-specific configurations
###########################################

skyrocket_configure_platform(compile_flags)
skyrocket_configure_graphics_api(graphics_api)
list(APPEND dependencies "${graphics_api}")

###### macOS ######
if (${skyrocket_platform} STREQUAL "macOS")
    MESSAGE("-- Skyrocket.Platform: Building for macOS")

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

    add_subdirectory(macOS)
endif ()

######################################
## Add library and link dependencies
######################################

skyrocket_add_library(${lib_name} STATIC)
target_link_libraries(${lib_name} "${dependencies}")

####################################
## Set compile options
####################################

target_compile_options(${lib_name} PRIVATE "${compile_flags}")


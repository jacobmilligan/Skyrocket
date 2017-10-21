set(CMAKE_CXX_STANDARD 14)

set(lib_name SkyrocketPlatform)

set(compile_flags)
set(dependencies)

###########################################
## Add dependencies from Skyrocket
###########################################

list(APPEND dependencies SkyrocketCore)

###########################################
## Platform-specific configurations
###########################################

MESSAGE("-- Skyrocket.Platform: Building for ${skyrocket_platform}")

###### macOS ######
if (${skyrocket_platform} STREQUAL "macOS")
    add_subdirectory(macOS)
elseif (${skyrocket_platform} STREQUAL "Windows")
    add_subdirectory(Windows)
endif ()

######################################
## Add sources
######################################

skyrocket_add_sources(Platform.cpp
        PlatformEvents.cpp
        Filesystem.cpp
        Time.cpp
        Thread.cpp)

######################################
## Add library and link dependencies
######################################

skyrocket_add_library(${lib_name} STATIC)
target_link_libraries(${lib_name} "${dependencies}")

####################################
## Set compile options
####################################
skyrocket_configure_platform(compile_flags)
target_compile_options(${lib_name} PRIVATE "${compile_flags}")


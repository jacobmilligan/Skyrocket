set(CMAKE_CXX_STANDARD 14)

set(lib_name SkyrocketPlatform)

skyrocket_add_sources(StdPlatform/StdPlatform.cpp)

set(compile_flags)
set(dependencies)

###########################################
## Add dependencies from Skyrocket
###########################################

list(APPEND dependencies SkyrocketCore)

######################################
## Add subdirectories and sources
######################################

add_subdirectory(Common)

###########################################
## Platform-specific configurations
###########################################

###### macOS ######
if (${skyrocket_platform} STREQUAL "macOS")
    add_subdirectory(macOS)
    add_subdirectory(StdPlatform)
endif ()

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


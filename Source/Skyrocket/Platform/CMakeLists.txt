set(CMAKE_CXX_STANDARD 14)

set(lib_name SkyrocketPlatform)

skyrocket_add_sources(Generic/ChronoTime.cpp)

set(compile_flags)
set(dependencies)

###########################################
## Add dependencies from Skyrocket
###########################################

list(APPEND dependencies SkyrocketCore)

######################################
## Add subdirectories and sources
######################################

add_subdirectory(Generic)

###########################################
## Platform-specific configurations
###########################################

skyrocket_configure_platform(compile_flags)

###### macOS ######
if (${skyrocket_platform} STREQUAL "macOS")
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


set(CMAKE_CXX_STANDARD 14)

set(lib_name SkyrocketResource)

set(compile_flags)
set(dependencies)

skyrocket_add_sources(Font.cpp)

######################################
## Add library and link dependencies
######################################

skyrocket_add_library(${lib_name} STATIC)
target_link_libraries(${lib_name} "${dependencies}")

######################################
## Add external deps
######################################

find_package(FreeType REQUIRED)
if (FREETYPE_FOUND)
    target_include_directories(${lib_name} PUBLIC ${FREETYPE_INCLUDE_DIRS})
    target_link_libraries(${lib_name} ${FREETYPE_LIBRARIES})
endif ()

####################################
## Set compile options
####################################
skyrocket_configure_platform(compile_flags)
target_compile_options(${lib_name} PRIVATE "${compile_flags}")
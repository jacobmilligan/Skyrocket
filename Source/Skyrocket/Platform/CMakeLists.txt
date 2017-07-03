set(lib_name SkyrocketPlatform)

skyrocket_add_sources(Cross/ChronoTime.cpp)

set(compile_flags)

if (APPLE)
    add_subdirectory(macOS)
endif ()

skyrocket_add_library(${lib_name} STATIC)
target_link_libraries(${lib_name} SkyrocketCore)

if (APPLE)
    # Treat all files as objective-c++
    set(compile_flags ${compile_flags} "-objective-c++")

    FIND_LIBRARY(cocoa_lib Cocoa)
    if (NOT cocoa_lib)
        MESSAGE("-- Skyrocket: Could not find Cocoa framework")
    else()
        target_link_libraries(${lib_name} ${cocoa_lib})
    endif ()

    FIND_LIBRARY(quartzcore_lib QuartzCore)
    if (NOT quartzcore_lib)
        MESSAGE("-- Skyrocket: Could not find QuartzCore framework")
    else()
        target_link_libraries(${lib_name} ${quartzcore_lib})
    endif ()

    if (SKYROCKET_GRAPHICS_API STREQUAL "Metal")
        FIND_LIBRARY(METAL_LIBRARY Metal)
        if (METAL_LIBRARY)
            MESSAGE("-- Skyrocket: Using the Metal API")
            target_link_libraries(${lib_name} ${METAL_LIBRARY})
        endif ()
    endif ()
endif ()

target_compile_options(${lib_name} PRIVATE "${compile_flags}")

skyrocket_add_sources(GDI.cpp
        Definitions.cpp)

add_subdirectory(Experimental)

if (skyrocket_graphics_api STREQUAL "Metal")
    add_subdirectory(Metal)
elseif (WIN32)
    add_subdirectory(Windows)
endif ()


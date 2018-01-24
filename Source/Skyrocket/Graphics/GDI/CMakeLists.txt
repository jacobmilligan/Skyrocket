skyrocket_add_sources(GDI.cpp
        Definitions.cpp)

if (SKYROCKET_EXPERIMENTAL)
    add_subdirectory(Experimental)
endif ()

if (skyrocket_graphics_api STREQUAL "Metal")
    add_subdirectory(Metal)
elseif (WIN32)
    add_subdirectory(Windows)
endif ()


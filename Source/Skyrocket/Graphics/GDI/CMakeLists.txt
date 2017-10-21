skyrocket_add_sources(GDI.cpp)

if (skyrocket_graphics_api STREQUAL "Metal")
    add_subdirectory(Metal)
elseif (WIN32)
    add_subdirectory(Windows)
endif ()


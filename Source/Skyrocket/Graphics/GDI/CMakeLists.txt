skyrocket_add_sources(GDI.cpp
        Definitions.cpp
        CommandList.cpp)

if ("Metal" IN_LIST skyrocket_graphics_api)
    add_subdirectory(Metal)
elseif (WIN32)
    add_subdirectory(Windows)
endif ()


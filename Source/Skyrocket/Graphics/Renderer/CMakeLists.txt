skyrocket_add_sources(GDI.cpp
        Definitions.cpp
        CommandList.cpp
        Renderer.cpp
        FrameInfo.cpp)

foreach(api IN LISTS skyrocket_graphics_api)
    add_subdirectory(${api})
endforeach()


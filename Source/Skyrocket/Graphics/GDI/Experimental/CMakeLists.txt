skyrocket_add_sources(CommandBuffer.cpp
        GraphicsDriver.cpp
        GDI_EX.cpp)

if (skyrocket_graphics_api STREQUAL "Metal")
    skyrocket_add_sources(Metal/MetalGDI.mm)
elseif (WIN32)
endif ()
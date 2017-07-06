skyrocket_add_sources(Platform.mm
        CocoaView.mm)

if (skyrocket_graphics_api STREQUAL "Metal")
    skyrocket_add_sources(MetalView.mm)
endif ()
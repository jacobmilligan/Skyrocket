skyrocket_add_sources(Platform.mm)

if (SKYROCKET_GRAPHICS_API STREQUAL "Metal")
    skyrocket_add_sources(MetalView.mm)
endif ()
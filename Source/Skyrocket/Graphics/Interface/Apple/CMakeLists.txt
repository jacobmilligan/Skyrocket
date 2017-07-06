skyrocket_add_sources(MacViewport.mm)

if (skyrocket_graphics_api STREQUAL "Metal")
    add_subdirectory(Metal)
endif()
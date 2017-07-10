MESSAGE("-- Skyrocket.Platform: Building for macOS")

# Add library dependencies
FIND_LIBRARY(cocoa_lib Cocoa)
if (NOT cocoa_lib)
    MESSAGE("-- Skyrocket.Platform: Could not find Cocoa framework")
else()
    list(APPEND dependencies ${cocoa_lib})
endif ()

skyrocket_add_sources(MacPlatform.mm
        CocoaWindow.mm
        MacApplication.mm
        MacInput.mm)
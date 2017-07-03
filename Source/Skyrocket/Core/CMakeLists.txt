skyrocket_add_sources(Diagnostics/Internal/ErrorMessage.cpp
        Diagnostics/Internal/Stopwatch.cpp
        Diagnostics/Internal/FPSCounter.cpp
        Diagnostics/Internal/SkyAssert.cpp)

skyrocket_add_library(SkyrocketCore STATIC)
target_link_libraries(SkyrocketCore SkyrocketPlatform)
skyrocket_add_sources(Diagnostics/Internal/Error.cpp
        Diagnostics/Internal/Stopwatch.cpp
        Diagnostics/Internal/FPSCounter.cpp)

skyrocket_add_library(SkyrocketCore STATIC)
target_link_libraries(SkyrocketCore SkyrocketPlatform)
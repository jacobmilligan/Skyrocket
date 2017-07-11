if (APPLE)
    add_subdirectory(Apple)
elseif(WIN32)
    add_subdirectory(Windows)
endif ()

skyrocket_add_sources(RenderResource.cpp)

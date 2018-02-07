skyrocket_add_sources(GLGDI.cpp
        GLResource.cpp)

if (APPLE)
    skyrocket_add_sources(NSGLContext.mm
            NSGLView.mm)
endif ()
//
//  OpenGLGDI.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 6/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Graphics/Renderer/OpenGL/GLGDI.hpp"
#include "Skyrocket/Graphics/Viewport.hpp"

#if SKY_OS_MACOS == 1

#include <OpenGL/gl3.h>

#endif

sky::OpenGLGDI::~OpenGLGDI()
{

}

bool sky::OpenGLGDI::init(sky::Viewport* viewport)
{
    {
        AssertGuard ag("Creating OpenGL context", nullptr);
        context_.create(viewport);
    }

    return true;
}

bool sky::OpenGLGDI::destroy()
{
    return GDI::destroy();
}

bool sky::OpenGLGDI::begin_frame(sky::FrameInfo* frame_info)
{
    return GDI::begin_frame(frame_info);
}

bool sky::OpenGLGDI::end_frame(sky::FrameInfo* frame_info)
{
    return GDI::end_frame(frame_info);
}

void sky::OpenGLGDI::set_viewport(sky::Viewport* viewport)
{
    GDI::set_viewport(viewport);
}

bool
sky::OpenGLGDI::create_vertex_buffer(uint32_t vbuf_id, const sky::MemoryBlock& initial_data, sky::BufferUsage usage)
{
    return GDI::create_vertex_buffer(vbuf_id, initial_data, usage);
}

bool sky::OpenGLGDI::set_vertex_buffer(uint32_t vbuf_id)
{
    return GDI::set_vertex_buffer(vbuf_id);
}

bool sky::OpenGLGDI::update_vertex_buffer(uint32_t vbuf_id, const sky::MemoryBlock& data)
{
    return GDI::update_vertex_buffer(vbuf_id, data);
}

bool sky::OpenGLGDI::create_index_buffer(uint32_t ibuf_id, const sky::MemoryBlock& initial_data)
{
    return GDI::create_index_buffer(ibuf_id, initial_data);
}

bool sky::OpenGLGDI::set_index_buffer(uint32_t ibuf_id)
{
    return GDI::set_index_buffer(ibuf_id);
}

bool sky::OpenGLGDI::draw()
{
    return GDI::draw();
}

bool sky::OpenGLGDI::draw_instanced(uint32_t instance)
{
    return GDI::draw_instanced(instance);
}

bool
sky::OpenGLGDI::create_program(uint32_t program_id, const sky::Path& vs_path, const sky::Path& frag_path)
{
    return GDI::create_program(program_id, vs_path, frag_path);
}

bool sky::OpenGLGDI::set_program(uint32_t program_id)
{
    return GDI::set_program(program_id);
}

bool sky::OpenGLGDI::create_uniform(uint32_t u_id, uint32_t size)
{
    return GDI::create_uniform(u_id, size);
}

bool sky::OpenGLGDI::set_uniform(uint32_t u_id, uint32_t index)
{
    return GDI::set_uniform(u_id, index);
}

bool sky::OpenGLGDI::update_uniform(uint32_t u_id, const sky::MemoryBlock& data, uint32_t offset)
{
    return GDI::update_uniform(u_id, data, offset);
}

bool
sky::OpenGLGDI::create_texture(uint32_t t_id, uint32_t width, uint32_t height, sky::PixelFormat::Enum pixel_format, bool mipmapped)
{
    return GDI::create_texture(t_id, width, height, pixel_format, mipmapped);
}

bool
sky::OpenGLGDI::create_texture_region(uint32_t tex_id, const sky::UIntRect& region, sky::PixelFormat::Enum pixel_format, uint8_t* data)
{
    return GDI::create_texture_region(tex_id, region, pixel_format, data);
}

bool sky::OpenGLGDI::set_texture(uint32_t t_id, uint32_t index)
{
    return GDI::set_texture(t_id, index);
}

bool sky::OpenGLGDI::set_state(uint32_t flags)
{
    return GDI::set_state(flags);
}

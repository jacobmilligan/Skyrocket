//
//  OpenGLGDI.HPP
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 6/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once


#include "Skyrocket/Graphics/Renderer/GDI.hpp"
#include "Skyrocket/Graphics/Renderer/OpenGL/GLContext.hpp"

namespace sky {


class OpenGLGDI : public GDI {
public:
    virtual ~OpenGLGDI();
    virtual bool init(Viewport* viewport);
    virtual bool destroy();
    virtual bool begin_frame(FrameInfo* frame_info);
    virtual bool end_frame(FrameInfo* frame_info);
    virtual void set_viewport(Viewport* viewport);
protected:
    virtual bool
    create_vertex_buffer(uint32_t vbuf_id, const MemoryBlock& initial_data, BufferUsage usage);
    virtual bool set_vertex_buffer(uint32_t vbuf_id);
    virtual bool update_vertex_buffer(uint32_t vbuf_id, const MemoryBlock& data);
    virtual bool create_index_buffer(uint32_t ibuf_id, const MemoryBlock& initial_data);
    virtual bool set_index_buffer(uint32_t ibuf_id);
    virtual bool draw();
    virtual bool draw_instanced(uint32_t instance);
    virtual bool create_program(uint32_t program_id, const Path& vs_path, const Path& frag_path);
    virtual bool set_program(uint32_t program_id);
    virtual bool create_uniform(uint32_t u_id, uint32_t size);
    virtual bool set_uniform(uint32_t u_id, uint32_t index);
    virtual bool update_uniform(uint32_t u_id, const MemoryBlock& data, uint32_t offset);
    virtual bool
    create_texture(uint32_t t_id, uint32_t width, uint32_t height, PixelFormat::Enum pixel_format, bool mipmapped);
    virtual bool
    create_texture_region(uint32_t tex_id, const UIntRect& region, PixelFormat::Enum pixel_format, uint8_t* data);
    virtual bool set_texture(uint32_t t_id, uint32_t index);
    virtual bool set_state(uint32_t flags);
private:
    GLContext context_;
};


} // namespace sky




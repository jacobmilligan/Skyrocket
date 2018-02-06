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
#include "Skyrocket/Core/Containers/HandleTable.hpp"
#include "Skyrocket/Graphics/Renderer/OpenGL/GLResource.hpp"

namespace sky {


class OpenGLGDI : public GDI {
public:
    ~OpenGLGDI();
    bool init(Viewport* viewport) override;
    bool destroy() override;
    bool begin_frame(FrameInfo* frame_info) override;
    bool end_frame(FrameInfo* frame_info) override;
    void set_viewport(Viewport* viewport) override;
protected:
    bool create_vertex_buffer(uint32_t vbuf_id, const MemoryBlock& initial_data,
                                      BufferUsage usage) override;
    bool set_vertex_buffer(uint32_t vbuf_id) override;
    bool update_vertex_buffer(uint32_t vbuf_id, const MemoryBlock& data) override;
    bool create_index_buffer(uint32_t ibuf_id, const MemoryBlock& initial_data) override;
    bool set_index_buffer(uint32_t ibuf_id) override;
    bool draw() override;
    bool draw_instanced(uint32_t instance) override;
    bool create_program(uint32_t program_id, const Path& vs_path, const Path& frag_path) override;
    bool set_program(uint32_t program_id) override;
    bool create_uniform(uint32_t u_id, uint32_t size) override;
    bool set_uniform(uint32_t u_id, uint32_t index) override;
    bool update_uniform(uint32_t u_id, const MemoryBlock& data, uint32_t offset) override;
    bool create_texture(uint32_t t_id, uint32_t width, uint32_t height,
                                PixelFormat::Enum pixel_format, bool mipmapped) override;
    bool create_texture_region(uint32_t tex_id, const UIntRect& region,
                                       PixelFormat::Enum pixel_format, uint8_t* data) override;
    bool set_texture(uint32_t t_id, uint32_t index) override;
    bool set_state(uint32_t flags) override;
private:
    GLContext context_;
    Viewport* viewport_{nullptr};
    GLuint default_vao_{0};

    HandleTable<GLuint, vertex_buffer_max> vertex_buffers_;
    HandleTable<GLuint, index_buffer_max> index_buffers_;
    HandleTable<GLuint, uniform_buffer_max> uniform_buffers_;
    HandleTable<GLProgram, shader_max> programs_;
    HandleTable<GLuint, texture_max> textures_;
};


} // namespace sky




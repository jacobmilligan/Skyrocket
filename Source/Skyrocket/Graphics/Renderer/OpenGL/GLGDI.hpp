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

struct GLUniformSlot {
    UniformType type;
    size_t size;
    void* data;
};

class OpenGLGDI : public GDI {
public:
    ~OpenGLGDI() override;
    bool init(Viewport* viewport) override;
    bool destroy() override;
    bool begin_frame(FrameInfo* frame_info) override;
    bool end_frame(FrameInfo* frame_info) override;
    void set_viewport(Viewport* viewport) override;
    void set_clear_color(const Color& color) override;
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

    bool create_uniform(uint32_t u_id, UniformType type, uint32_t size) override;
    bool set_uniform(uint32_t u_id, uint32_t index) override;
    bool update_uniform(uint32_t u_id, const MemoryBlock& data, uint32_t offset) override;

    bool create_instance_buffer(uint32_t inst_id, uint32_t stride, uint32_t size) override;
    bool update_instance_buffer(uint32_t inst_id, uint8_t* data, uint32_t index) override;
    bool set_instance_buffer(uint32_t inst_id, uint32_t index) override;

    bool create_texture(uint32_t t_id, uint32_t width, uint32_t height,
                                PixelFormat::Enum pixel_format, bool mipmapped) override;
    bool create_texture_region(uint32_t tex_id, const UIntRect& region,
                                       PixelFormat::Enum pixel_format, uint8_t* data) override;
    bool set_texture(uint32_t t_id, uint32_t index) override;

    bool set_state(uint32_t flags) override;
private:
    struct GLPixelFormat {
        GLenum internal_format;
        GLenum data_format;
        size_t bytes;
    };

    static constexpr GLPixelFormat gl_pixel_formats_[] = {
        {GL_R8,                 GL_RED},                    // r8
        {GL_R16,                GL_RED},                    // r16
        {GL_R32F,               GL_RED},                    // r32
        {GL_RG8,                GL_RG},                     // rg8
        {GL_RG16,               GL_RG},                     // rg16
        {GL_RG32F,              GL_RG},                     // rg32
        {GL_RGB8,               GL_RGB},                    // rgb8
        {GL_BGRA,               GL_BGRA},                   // bgra8
        {GL_RGBA8,              GL_RGBA},                   // rgba8
        {GL_RGBA16,             GL_RGBA},                   // rgba16
        {GL_RGBA32F,            GL_RGBA},                   // rgba32
        {GL_DEPTH_COMPONENT32,  GL_DEPTH_COMPONENT},        // depth
        {GL_STENCIL_INDEX8,     GL_STENCIL_INDEX},          // stencil
        {GL_FALSE,              GL_FALSE}                   // unknown
    };

    static constexpr size_t gl_pixelfmt_tablesize = sizeof(gl_pixel_formats_) / sizeof(GLPixelFormat);

    static_assert(gl_pixelfmt_tablesize == static_cast<size_t>(PixelFormat::Enum::unknown) + 1,
                  "Skyrocket: OpenGL error: the translation table for PixelFormat "
                      "is missing entries. Please update to sync with the PixelFormat enum.");

    GLContext context_;
    Viewport* viewport_{nullptr};
    GLuint default_vao_{0};
    GLProgram default_program_;

    HandleTable<GLuint, vertex_buffer_max> vertex_buffers_;
    HandleTable<GLuint, index_buffer_max> index_buffers_;
    HandleTable<GLUniformSlot, uniform_buffer_max> uniform_buffers_;
    HandleTable<GLInstanceBuffer, instance_buffer_max> instance_buffers_;
    HandleTable<GLProgram, shader_max> programs_;
    HandleTable<GLuint, texture_max> textures_;

    void set_uniform_data(GLint location, GLUniformSlot& slot);
    bool check_uniform_slots();
};


} // namespace sky




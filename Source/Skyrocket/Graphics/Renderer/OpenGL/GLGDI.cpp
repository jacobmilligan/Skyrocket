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
#include "Skyrocket/Graphics/Renderer/Vertex.hpp"
#include "Skyrocket/Platform/Filesystem.hpp"


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
        viewport_ = viewport;
        context_.create(viewport_);

        // Reset errors
        GLenum err;
        do {
            err = glGetError();
        } while (err != GL_NO_ERROR);
    }

    SKY_GL_CHECK_ERROR(glEnable(GL_BLEND));
    SKY_GL_CHECK_ERROR(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    SKY_GL_CHECK_ERROR(glGenVertexArrays(1, &default_vao_));

    return true;
}

bool sky::OpenGLGDI::destroy()
{
    context_.destroy();
    return true;
}

bool sky::OpenGLGDI::begin_frame(sky::FrameInfo* frame_info)
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (!viewport_->is_open()) {
        return false;
    }

    SKY_GL_CHECK_ERROR(glBindVertexArray(default_vao_));

    return true;
}

bool sky::OpenGLGDI::end_frame(sky::FrameInfo* frame_info)
{
    SKY_GL_CHECK_ERROR(glBindVertexArray(0));

    context_.swap_buffers();

    return true;
}

void sky::OpenGLGDI::set_viewport(sky::Viewport* viewport)
{
    viewport_ = viewport;
    context_.set_view(viewport);
    SKY_GL_CHECK_ERROR(glViewport(0, 0,
                                  static_cast<GLsizei>(viewport->size().x),
                                  static_cast<GLsizei>(viewport->size().y)));
}

bool sky::OpenGLGDI::create_vertex_buffer(uint32_t vbuf_id, const sky::MemoryBlock& initial_data,
                                          sky::BufferUsage usage)
{
    auto glbuf = vertex_buffers_.create(vbuf_id);
    if (glbuf == nullptr) {
        return false;
    }

    GLenum glusage = (usage == BufferUsage::staticbuf) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;

    SKY_GL_CHECK_ERROR(glGenBuffers(1, glbuf));
    SKY_GL_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, *glbuf));
    SKY_GL_CHECK_ERROR(glBufferData(GL_ARRAY_BUFFER, initial_data.size, initial_data.data, glusage));
    SKY_GL_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, 0));

    return true;
}

bool sky::OpenGLGDI::set_vertex_buffer(uint32_t vbuf_id)
{
    auto glbuf = vertex_buffers_.get(vbuf_id);
    if (glbuf == nullptr) {
        return false;
    }

    SKY_GL_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, *glbuf));

    // TODO (Jacob): needs to be changed once the rest works because resetting attribs each frame is expensive
    // Position
    SKY_GL_CHECK_ERROR(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr));
    SKY_GL_CHECK_ERROR(glEnableVertexAttribArray(0));

    // Color
    auto offset = reinterpret_cast<void*>(4 * sizeof(float));
    SKY_GL_CHECK_ERROR(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), offset));
    SKY_GL_CHECK_ERROR(glEnableVertexAttribArray(1));

    // Texture coordinates
    offset = reinterpret_cast<void*>(8 * sizeof(float));
    SKY_GL_CHECK_ERROR(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offset));
    SKY_GL_CHECK_ERROR(glEnableVertexAttribArray(2));

    return true;
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
    if (state_.vertex_buffer <= 0) {
        return false;
    }

    if (state_.index_buffer > 0) {
        // TODO (Jacob): draw indexed
        return true;
    }

    SKY_GL_CHECK_ERROR(glDrawArrays(GL_TRIANGLES, state_.vertex_offset, state_.vertex_count));
    return true;
}

bool sky::OpenGLGDI::draw_instanced(uint32_t instance)
{
    return GDI::draw_instanced(instance);
}

bool
sky::OpenGLGDI::create_program(uint32_t program_id, const sky::Path& vs_path, const sky::Path& frag_path)
{
    auto glprog = programs_.create(program_id);
    if (glprog == nullptr) {
        return false;
    }

    auto vs_source = fs::slurp_file(vs_path);
    auto frag_source = fs::slurp_file(frag_path);
    return glprog->create(vs_source.c_str(), frag_source.c_str());
}

bool sky::OpenGLGDI::set_program(uint32_t program_id)
{
    auto glprog = programs_.get(program_id);
    if (glprog == nullptr) {
        SKY_ERROR("GDI", "Invalid program id %d specified", program_id);
        return false;
    }

    SKY_GL_CHECK_ERROR(glUseProgram(glprog->id));
    return true;
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

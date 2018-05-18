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
#include "Skyrocket/Core/Math/Matrix4.hpp"


#if SKY_OS_MACOS == 1

#include <OpenGL/gl3.h>

#endif

namespace sky {

#if SKY_COMPILER_MSVC != 1

constexpr OpenGLGDI::GLPixelFormat OpenGLGDI::gl_pixel_formats_[];

#endif


void set_uniform_data_vec1(const GLUniformInfo& info, const GLUniformSlot& slot)
{
    auto vec = static_cast<float*>(slot.data);
    SKY_GL_CHECK(glUniform1f(info.location, *vec));
}

void set_uniform_data_vec2(const GLUniformInfo& info, const GLUniformSlot& slot)
{
    auto vec = static_cast<Vector2f*>(slot.data);
    SKY_GL_CHECK(glUniform2f(info.location, vec->x, vec->y));
}

void set_uniform_data_vec3(const GLUniformInfo& info, const GLUniformSlot& slot)
{
    auto vec = static_cast<Vector3f*>(slot.data);
    SKY_GL_CHECK(glUniform3f(info.location, vec->x, vec->y, vec->z));
}

void set_uniform_data_vec4(const GLUniformInfo& info, const GLUniformSlot& slot)
{
    auto vec = static_cast<Vector4f*>(slot.data);
    SKY_GL_CHECK(glUniform4f(info.location, vec->x, vec->y, vec->z, vec->w));
}

void set_uniform_data_mat2(const GLUniformInfo& info, const GLUniformSlot& slot)
{
    SKY_ERROR("OpenGL", "Uniform data for 2x2 matrices is unimplemented");
}

void set_uniform_data_mat3(const GLUniformInfo& info, const GLUniformSlot& slot)
{
    SKY_ERROR("OpenGL", "Uniform data for 3x3 matrices is unimplemented");
}

void set_uniform_data_mat4(const GLUniformInfo& info, const GLUniformSlot& slot)
{
    auto mat = static_cast<Matrix4f*>(slot.data);
    if (info.type != UniformType::mat4) {
        SKY_GL_CHECK(glUniform4fv(info.location, 4, mat->entries));
        return;
    }
    SKY_GL_CHECK(glUniformMatrix4fv(info.location, 1, GL_FALSE, mat->entries));
}

void set_uniform_data_tex1d(const GLUniformInfo& info, const GLUniformSlot& slot)
{
    SKY_ERROR("OpenGL", "Uniform data for tex1d is unimplemented");
}

void set_uniform_data_tex2d(const GLUniformInfo& info, const GLUniformSlot& slot)
{
    SKY_ERROR("OpenGL", "Uniform data for tex2d is unimplemented");
}

void set_uniform_data_tex3d(const GLUniformInfo& info, const GLUniformSlot& slot)
{
    SKY_ERROR("OpenGL", "Uniform data for tex3d is unimplemented");
}

void set_uniform_data_cubemap(const GLUniformInfo& info, const GLUniformSlot& slot)
{
    SKY_ERROR("OpenGL", "Uniform data for cubemaps is unimplemented");
}


OpenGLGDI::~OpenGLGDI() = default;

void OpenGLGDI::set_uniform_data(const GLUniformInfo& info, const GLUniformSlot& slot)
{
    using set_uniform_data_func_t = void (*)(const GLUniformInfo&, const GLUniformSlot&);

    static constexpr set_uniform_data_func_t func_table[] = {
        &set_uniform_data_vec1, // vec1
        &set_uniform_data_vec2, // vec2
        &set_uniform_data_vec3, // vec3
        &set_uniform_data_vec4, // vec4
        &set_uniform_data_mat2, // mat2
        &set_uniform_data_mat3, // mat3
        &set_uniform_data_mat4, // mat4
        &set_uniform_data_tex1d, // tex1d
        &set_uniform_data_tex2d, // tex2d
        &set_uniform_data_tex3d, // tex3d
        &set_uniform_data_cubemap, // cubemap
    };

    static constexpr size_t table_size = sizeof(func_table) / sizeof(set_uniform_data_func_t);

    static_assert(table_size == static_cast<size_t>(UniformType::unknown), "Skyrocket: OpenGL "
        "error: the translation table for UniformType in `set_uniform_data` is missing entries. "
        "Please update to sync with the UniformType enum");

    return func_table[static_cast<size_t>(slot.type)](info, slot);
}

bool OpenGLGDI::init(Viewport* viewport)
{
    Matrix::depth = ClipSpaceDepth::negative_one_to_one;

    {
        AssertGuard ag("Creating OpenGL context", nullptr);
        viewport_ = viewport;
        context_.create();

        // Reset errors
        GLenum err;
        do {
            err = glGetError();
        } while (err != GL_NO_ERROR);
    }

    // Enable settings
    SKY_GL_CHECK(glEnable(GL_BLEND));
    SKY_GL_CHECK(glEnable(GL_CULL_FACE));
    SKY_GL_CHECK(glEnable(GL_DEPTH_TEST));
//    SKY_GL_CHECK(glFrontFace(GL_CW));

    SKY_GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    SKY_GL_CHECK(glGenVertexArrays(1, &default_vao_));

    const char* basic_vert = R"(
#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 1) in vec2 tex;

out vec4 frag_color;

void main() {
	gl_Position = position;
	frag_color = color;
}
)";

    const char* basic_frag = R"(
#version 330 core

out vec4 color;

in vec4 frag_color;

void main() {
	color = frag_color;
}
)";

    default_program_.create(basic_vert, basic_frag);
    return true;
}

bool OpenGLGDI::destroy()
{
    context_.destroy();
    return true;
}

bool OpenGLGDI::begin_frame(FrameInfo* frame_info)
{
    if (!viewport_->is_open()) {
        return false;
    }

    if (!context_.has_view()) {
        context_.set_view(viewport_);
    }

    SKY_GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    SKY_GL_CHECK(glBindVertexArray(default_vao_));

    return true;
}

bool OpenGLGDI::end_frame(FrameInfo* frame_info)
{
    SKY_GL_CHECK(glBindVertexArray(0));
    frame_info->cpu_end();
    frame_info->gpu_start();
    context_.swap_buffers();
    frame_info->gpu_end();
    return true;
}

void OpenGLGDI::set_viewport(Viewport* viewport)
{
    viewport_ = viewport;
    context_.set_view(viewport);
    SKY_GL_CHECK(glViewport(0, 0, static_cast<GLsizei>(viewport->size().x),
                            static_cast<GLsizei>(viewport->size().y)));
}

bool OpenGLGDI::create_vertex_buffer(uint32_t vbuf_id, const MemoryBlock& initial_data,
                                          BufferUsage usage)
{
    auto glbuf = vertex_buffers_.create(vbuf_id);
    if (glbuf == nullptr) {
        return false;
    }

    GLenum glusage = (usage == BufferUsage::staticbuf) ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;

    SKY_GL_CHECK(glGenBuffers(1, glbuf));
    SKY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, *glbuf));
    SKY_GL_CHECK(glBufferData(GL_ARRAY_BUFFER, initial_data.size, initial_data.data, glusage));
    SKY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

    return true;
}

bool OpenGLGDI::set_vertex_buffer(uint32_t vbuf_id)
{
    auto glbuf = vertex_buffers_.get(vbuf_id);
    if (glbuf == nullptr) {
        return false;
    }

    SKY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, *glbuf));

    // TODO (Jacob): needs to be changed once the rest works because resetting attribs each frame is expensive
    // Position
    SKY_GL_CHECK(glEnableVertexAttribArray(0));
    SKY_GL_CHECK(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr));

    // Color
    auto offset = reinterpret_cast<void*>(4 * sizeof(float));
    SKY_GL_CHECK(glEnableVertexAttribArray(1));
    SKY_GL_CHECK(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), offset));

    // Texture coordinates
    offset = reinterpret_cast<void*>(8 * sizeof(float));
    SKY_GL_CHECK(glEnableVertexAttribArray(2));
    SKY_GL_CHECK(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offset));

    return true;
}

bool OpenGLGDI::update_vertex_buffer(uint32_t vbuf_id, const MemoryBlock& data)
{
    auto glbuf = vertex_buffers_.get(vbuf_id);
    if (glbuf == nullptr) {
        return false;
    }

    if (state_.vertex_buffer != *glbuf) {
        SKY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, *glbuf));
    }

    SKY_GL_CHECK(glBufferData(GL_ARRAY_BUFFER, data.size, data.data, GL_STATIC_DRAW));

    if (state_.vertex_buffer != *glbuf) {
        SKY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, state_.vertex_buffer));
    }
    return true;
}

bool OpenGLGDI::create_index_buffer(uint32_t ibuf_id, const MemoryBlock& initial_data)
{
    auto glbuf = index_buffers_.create(ibuf_id);
    if (glbuf == nullptr) {
        return false;
    }

    SKY_GL_CHECK(glGenBuffers(1, glbuf));
    if (*glbuf == 0) {
        return false;
    }

    SKY_GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *glbuf));
    SKY_GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, initial_data.size,
                                    initial_data.data, GL_STATIC_DRAW));
    SKY_GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    return true;
}

bool OpenGLGDI::set_index_buffer(uint32_t ibuf_id)
{
    auto glbuf = index_buffers_.get(ibuf_id);
    if (glbuf == nullptr) {
        return false;
    }

    SKY_GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *glbuf));
    return true;
}

bool
OpenGLGDI::create_program(uint32_t program_id, const shadecc::ShaderSource& vs_src, const shadecc::ShaderSource& fs_src)
{
    auto glprog = programs_.create(program_id);
    if (glprog == nullptr) {
        return false;
    }

    return glprog->create(vs_src.glsl_src, fs_src.glsl_src);
}

bool OpenGLGDI::set_program(uint32_t program_id)
{
    if (program_id == 0) {
        SKY_GL_CHECK(glUseProgram(default_program_.id));
        return true;
    }

    auto glprog = programs_.get(program_id);
    if (glprog == nullptr) {
        SKY_ERROR("GDI", "Invalid program id %d specified", program_id);
        return false;
    }

    SKY_GL_CHECK(glUseProgram(glprog->id));
    return true;
}

bool OpenGLGDI::create_uniform(uint32_t u_id, const char* name, uint32_t size, UniformType type)
{
    auto buf = uniform_buffers_.create(u_id);
    if (buf == nullptr) {
        return false;
    }

    auto namelen = strlen(name);
    buf->location = -1;
    buf->size = size;
    buf->type = type;
    buf->data = malloc(size);

    // Keep normal name for samplers as they're not declared in uniform blocks
    if (type == UniformType::tex2d || type == UniformType::tex1d || type == UniformType::tex3d) {
        strcpy(buf->name, name);
        return true;
    }

    size_t block_len = 0;
    size_t member_start = 0;
    for (int c = 0; c < namelen; ++c) {
        if (name[c] == '.') {
            strncpy(buf->block, name, block_len);
            member_start = block_len + 1;
        }

        ++block_len;
    }

    SKY_ASSERT(member_start != 0, "Invalid format for uniform. "
                                  "Must be specified as <uniform_block_name>.<uniform_name>, "
                                  "i.e: `Params.model`")

    strcpy(buf->name, name + member_start);
    return true;
}

bool OpenGLGDI::set_uniform(uint32_t u_id, uint32_t index)
{
    // handled by `GDI::submit`
    auto uniform = uniform_buffers_.get(u_id);
    if (uniform == nullptr) {
        return false;
    }

    if (uniform->location < 0) {
        auto prog = programs_.get(state_.program);
        SKY_GL_CHECK(uniform->location = glGetUniformLocation(prog->id, uniform->block));
        if (uniform->location < 0) {
            SKY_ERROR("Renderer", "No uniform with the name `%s` was found in the set program",
                      uniform->name);
            return false;
        }
    }

    state_.uniform_slots[uniform->location] = u_id;

    return true;
}

bool OpenGLGDI::update_uniform(uint32_t u_id, const MemoryBlock& data, uint32_t offset)
{
    auto buf = uniform_buffers_.get(u_id);
    if (buf == nullptr) {
        return false;
    }

    if (data.size > buf->size) {
        SKY_ERROR("OpenGL", "New uniform data size (%zu) is large than the size allocated for the "
            "uniform in `create_uniform` (%zu)", data.size, buf->size);
        return false;
    }

    buf->size = data.size;
    auto dest = static_cast<uint8_t*>(buf->data);
    memcpy(dest + offset, data.data, buf->size);

    return true;
}

bool OpenGLGDI::create_instance_buffer(uint32_t inst_id, uint32_t stride, uint32_t size)
{
    auto buf = instance_buffers_.create(inst_id);
    if (buf == nullptr) {
        return false;
    }

    buf->bytes = size * stride;
    buf->data = static_cast<uint8_t*>(malloc(buf->bytes));
    buf->stride = stride;

    SKY_GL_CHECK(glGenBuffers(1, &buf->id));
    if (buf->id == 0) {
        return false;
    }

    SKY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buf->id));
    SKY_GL_CHECK(glBufferData(GL_ARRAY_BUFFER, buf->bytes, nullptr, GL_DYNAMIC_DRAW));
    SKY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, state_.vertex_buffer));

    return true;
}

bool OpenGLGDI::update_instance_buffer(uint32_t inst_id, uint8_t* data, uint32_t index)
{
    auto buf = instance_buffers_.get(inst_id);
    if (buf == nullptr) {
        return false;
    }

    if (index > buf->bytes / buf->stride) {
        return false;
    }

    memcpy(buf->data + (index * buf->stride), data, buf->stride);
    return true;
}

bool OpenGLGDI::set_instance_buffer(uint32_t inst_id, uint32_t index)
{
//    auto buf = instance_buffers_.get(inst_id);
//    if (buf == nullptr) {
//        return false;
//    }
//
//    auto num_components = static_cast<GLint>(buf->stride / 16);
//    size_t offset = 0;
//    for (int i = 0; i < num_components; ++i) {
//        offset = num_components * i * sizeof(float);
//        SKY_GL_CHECK(glEnableVertexAttribArray(index + i));
//        SKY_GL_CHECK(glVertexAttribPointer(index + i, num_components,
//                                           GL_FLOAT, GL_FALSE, static_cast<GLsizei>(buf->stride),
//                                           reinterpret_cast<GLvoid*>(offset)));
//        SKY_GL_CHECK(glVertexAttribDivisor(index + i, 1));
//    }
//
//    SKY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buf->id));
//
//    return true;
    return GDI::set_instance_buffer(inst_id, index);
}

bool OpenGLGDI::create_texture(uint32_t t_id, uint32_t width, uint32_t height,
                               PixelFormat pixel_format, bool mipmapped)
{
    auto tex = textures_.create(t_id);
    if (tex == nullptr) {
        return false;
    }

    SKY_GL_CHECK(glGenTextures(1, tex));
    if (*tex == 0) {
        return false;
    }
    SKY_GL_CHECK(glBindTexture(GL_TEXTURE_2D, *tex));

    // Specify no mipmap levels for the texture but keep mipmap filtering on.
    // OpenGL textures have a default max mipmap level of 1000 until specified otherwise - causing
    // the texture to be considered incomplete if no mipmap levels are defined in the meantime.
    SKY_GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0));
    SKY_GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0));

    // Generate blank texture to be filled with `create_texture_region`
    auto pxlfmt = gl_pixel_formats_[flag_type(pixel_format)];
    SKY_GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, pxlfmt.internal_format, width, height, 0,
                                    pxlfmt.data_format, GL_UNSIGNED_BYTE, nullptr));

    SKY_GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
    return true;
}

bool OpenGLGDI::create_texture_region(uint32_t tex_id, const UIntRect& region,
                                      PixelFormat pixel_format, uint8_t* data)
{
    auto tex = textures_.get(tex_id);
    if (tex == nullptr) {
        return false;
    }

    auto gl_pxlfmt = gl_pixel_formats_[flag_type(pixel_format)];
    auto bpp = pf_bytes_per_pixel(pixel_format);

    // Adjust unpack alignment for 8 and 16 bit pixel formats
    SKY_GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, bpp));
    SKY_GL_CHECK(glBindTexture(GL_TEXTURE_2D, *tex));
    SKY_GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, region.position.x, region.position.y,
                                       region.width, region.height, gl_pxlfmt.data_format,
                                       GL_UNSIGNED_BYTE, data));
    SKY_GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

    if (bpp != 4) {
        SKY_GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));
    }
    return true;
}

bool OpenGLGDI::set_texture(uint32_t t_id, uint32_t index)
{
    auto tex = textures_.get(t_id);
    if (tex == nullptr) {
        return false;
    }

    SKY_GL_CHECK(glBindTexture(GL_TEXTURE_2D, *tex));
    return true;
}

bool OpenGLGDI::set_state(uint32_t flags)
{
    if ( ( 0 | flag_type(RenderPipelineState::culling_none)
        | flag_type(RenderPipelineState::culling_backface)
        | flag_type(RenderPipelineState::culling_frontface))
        & flags ) {

        if ( flag_type(RenderPipelineState::culling_none)& flags ) {
            SKY_GL_CHECK(glDisable(GL_CULL_FACE));
        }

        if ( flag_type(RenderPipelineState::culling_backface) & flags ) {
            SKY_GL_CHECK(glEnable(GL_CULL_FACE));
            SKY_GL_CHECK(glCullFace(GL_BACK));
        }

        if ( flag_type(RenderPipelineState::culling_frontface) & flags ) {
            SKY_GL_CHECK(glEnable(GL_CULL_FACE));
            SKY_GL_CHECK(glCullFace(GL_FRONT));
        }

    }

    return true;
}

bool OpenGLGDI::check_uniform_slots()
{
    GLProgram* prog = nullptr;

    if (state_.program == 0) {
        prog = &default_program_;
    } else {
        prog = programs_.get(state_.program);
    }

    if (prog == nullptr) {
        return false;
    }

    // Check all available uniforms in the current program to see which ones are occupying a
    // uniform slot in the render state, i.e. are currently bound.
    for (auto& info : prog->uniforms) {
        // We shouldn't need to check textures
        if (info.type == UniformType::tex2d) {
            continue;
        }

        auto handle = state_.uniform_slots[info.location];
        auto uniform = uniform_buffers_.get(handle);
        if (handle == 0 || uniform == nullptr) {
            // Uniform isn't bound
            SKY_ERROR("OpenGL", "Uniform data with name '%s' at slot %d is missing for program "
                "with id %d", info.name, info.location, state_.program);
            continue;
        }

        // The uniform is bound and needs its data updated
        // TODO(Jacob): This should really occur in `update_uniform`
        set_uniform_data(info, *uniform);
    }

    return true;
}

bool OpenGLGDI::draw()
{
    if (state_.vertex_buffer <= 0) {
        return false;
    }

    if (!check_uniform_slots()) {
        return false;
    }

    if (state_.index_buffer > 0) {
        SKY_GL_CHECK(
            glDrawElements(GL_TRIANGLES, state_.index_count, GL_UNSIGNED_INT, nullptr)
        );
        return true;
    }

    SKY_GL_CHECK(glDrawArrays(GL_TRIANGLES, state_.vertex_offset, state_.vertex_count));
    return true;
}

bool OpenGLGDI::draw_instanced(uint32_t instance)
{
    if (state_.vertex_buffer <= 0) {
        return false;
    }

    if (!check_uniform_slots()) {
        return false;
    }

    GLProgram* prog = nullptr;

    if (state_.program == 0) {
        prog = &default_program_;
    } else {
        prog = programs_.get(state_.program);
    }

    if (prog == nullptr) {
        return false;
    }

    for (auto& inst : prog->instances) {
        auto buf = instance_buffers_.get(state_.instance_slots[inst.index]);
        if (buf == nullptr) {
            continue;
        }

        // TODO(Jacob): Make sure it's really obvious that instance buffer elements have to be 16 byte multiples
        // Below: dirty hack to get OpenGL to play nice with other API's - elements must be
        // a 16 byte multiple
        auto component_size = static_cast<GLint>(buf->stride) / 16;

        SKY_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, buf->id));

        for (int i = 0; i < component_size; ++i) {
            SKY_GL_CHECK(glEnableVertexAttribArray(static_cast<GLuint>(inst.location + i)));
            SKY_GL_CHECK(glVertexAttribPointer(static_cast<GLuint>(inst.location + i),
                                               component_size,
                                               GL_FLOAT,
                                               GL_FALSE,
                                               static_cast<GLsizei>(buf->stride),
                                               reinterpret_cast<void*>(i * component_size * sizeof(float))));
            SKY_GL_CHECK(glVertexAttribDivisor(static_cast<GLuint>(inst.location + i), 1));
        }

        SKY_GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, instance * buf->stride, buf->data));
    }

    if (state_.index_buffer > 0) {
        SKY_GL_CHECK(
            glDrawElementsInstanced(GL_TRIANGLES, state_.index_count, GL_UNSIGNED_INT, nullptr,
                                    instance)
        );
        return true;
    }

    SKY_GL_CHECK(glDrawArraysInstanced(GL_TRIANGLES, state_.vertex_offset,
                                             state_.vertex_count, instance));
    return true;
}


} // namespace sky

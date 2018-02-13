//
//  GLResource.HPP
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 6/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Core/Diagnostics/Error.hpp"
#include "Skyrocket/Graphics/Renderer/OpenGL/GLConfig.hpp"

namespace sky {

enum class GLShaderType {
    vertex = GL_VERTEX_SHADER,
    fragment = GL_FRAGMENT_SHADER,
    geometry = GL_GEOMETRY_SHADER,
    unknown = 0
};

const char* get_shader_type_string(GLShaderType type);

void get_uniform_info();

template <GLShaderType T>
struct GLShader {
    GLuint id;

    GLShader()
        : id(0)
    {}

    bool create(const char* source);

    void destroy();
};

struct GLUniformInfo {
    static constexpr size_t max_name = 64;

    GLuint program;
    GLuint index;
    GLint location;
    UniformType type;
    GLint size;

    GLint name_len;
    GLint block_index;
    GLint block_offset;
    GLint array_stride;
    GLint matrix_stride;

    GLchar name[max_name];
};

struct GLProgram {
    GLuint id;

    GLint num_uniforms, num_attrs;
    std::vector<GLUniformInfo> uniforms;
    std::vector<GLUniformInfo> instances;

    GLProgram()
        : id(0),
          num_uniforms(0),
          num_attrs(0)
    {}

    GLProgram(const GLProgram& other) = delete;

    GLProgram(GLProgram&& other) noexcept
        : id(other.id),
          num_uniforms(other.num_uniforms),
          num_attrs(other.num_attrs)
    {
        other.num_uniforms = 0;
        other.id = 0;
        other.num_attrs = 0;
        std::swap(uniforms, other.uniforms);
    }

    GLProgram& operator=(const GLProgram& other) = default;

    GLProgram& operator=(GLProgram&& other) noexcept
    {
        id = other.id;
        num_uniforms = other.num_uniforms;
        num_attrs = other.num_attrs;

        other.num_uniforms = 0;
        other.id = 0;
        other.num_attrs = 0;

        std::swap(uniforms, other.uniforms);
        return *this;
    }

    bool create(const char* vertex_source, const char* fragment_source);

    void destroy();

private:
    void get_uniform_info(uint32_t index, GLUniformInfo* info);
};

struct GLInstanceBuffer {
    GLuint id;
    size_t stride;
    size_t bytes;
    uint8_t* data;
};


} // namespace sky

#include "Skyrocket/Graphics/Renderer/OpenGL/GLResource.inl"
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
    GLuint location;
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

    size_t num_uniforms;
    GLUniformInfo* uniforms;

    GLProgram()
        : id(0),
          num_uniforms(0),
          uniforms(nullptr)
    {}

    ~GLProgram()
    {
        delete [] uniforms;
    }

    GLProgram(const GLProgram& other) = delete;

    GLProgram(GLProgram&& other) noexcept
        : id(other.id),
          num_uniforms(other.num_uniforms),
          uniforms(nullptr)
    {
        other.num_uniforms = 0;
        other.id = 0;
        std::swap(uniforms, other.uniforms);
    }

    GLProgram& operator=(const GLProgram& other) = default;

    GLProgram& operator=(GLProgram&& other) noexcept
    {
        uniforms = nullptr;
        id = other.id;
        num_uniforms = other.num_uniforms;

        other.num_uniforms = 0;
        other.id = 0;

        std::swap(uniforms, other.uniforms);
        return *this;
    }

    bool create(const char* vertex_source, const char* fragment_source);

    void destroy();
};


} // namespace sky

#include "Skyrocket/Graphics/Renderer/OpenGL/GLResource.inl"
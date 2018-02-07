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

inline const char* get_shader_type_string(const GLShaderType type)
{
    switch (type) {
        case GLShaderType::vertex: return "vertex";
        case GLShaderType::fragment: return "fragment";
        case GLShaderType::geometry: return "geometry";
        case GLShaderType::unknown: break;
    }

    return "unknown shader type";
}

template <GLShaderType T>
struct GLShader {
    GLuint id;

    GLShader()
        : id(0)
    {}

    bool create(const char* source)
    {
        auto type = static_cast<GLenum>(T);
        id = glCreateShader(type);
        if (id == 0) {
            SKY_ERROR("OpenGL", "Failed to create %s shader resource", get_shader_type_string(T));
            return false;
        }

        SKY_GL_CHECK_ERROR(glShaderSource(id, 1, &source, NULL));

        SKY_GL_CHECK_ERROR(glCompileShader(id));

        GLint success;
        SKY_GL_CHECK_ERROR(glGetShaderiv(id, GL_COMPILE_STATUS, &success));
        if (!success) {
            const size_t log_size = 512;
            GLchar info_log[log_size];
            glGetShaderInfoLog(id, log_size, nullptr, info_log);
            SKY_ERROR("OpenGL", "Couldn't compile shader with id %d.\n%s", id, info_log);
            return false;
        }

        return true;
    }

    void destroy()
    {
        if (id != 0) {
            SKY_GL_CHECK_ERROR(glDeleteShader(id));
            id = 0;
        }
    }
};

struct GLProgram {
    GLuint id;

    GLProgram()
        : id(0)
    {}

    bool create(const char* vertex_source, const char* fragment_source)
    {
        GLShader<GLShaderType::vertex> vertex;
        GLShader<GLShaderType::fragment> fragment;

        if (!vertex.create(vertex_source) || !fragment.create(fragment_source)) {
            return false;
        }

        id = glCreateProgram();
        if (id == 0) {
            SKY_ERROR("OpenGL", "Failed to create program resource");
            return false;
        }

        SKY_GL_CHECK_ERROR(glAttachShader(id, vertex.id));
        SKY_GL_CHECK_ERROR(glAttachShader(id, fragment.id));
        SKY_GL_CHECK_ERROR(glLinkProgram(id));

        GLint success;
        SKY_GL_CHECK_ERROR(glGetProgramiv(id, GL_LINK_STATUS, &success));
        if (!success) {
            const size_t log_size = 512;
            GLchar info_log[log_size];
            glGetProgramInfoLog(id, log_size, nullptr, info_log);
            SKY_ERROR("OpenGL", "Failed to link shaders for program with id %d: %s", id, info_log);
            return false;
        }

        SKY_GL_CHECK_ERROR(glDetachShader(id, vertex.id));
        SKY_GL_CHECK_ERROR(glDetachShader(id, fragment.id));

        vertex.destroy();
        fragment.destroy();

        return true;
    }

    void destroy()
    {
        if (id != 0) {
            SKY_GL_CHECK_ERROR(glDeleteShader(id));
            id = 0;
        }
    }
};


}
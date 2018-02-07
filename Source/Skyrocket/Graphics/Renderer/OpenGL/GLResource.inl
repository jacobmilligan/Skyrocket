//
//  GLResource.inl
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 7/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

namespace sky {


template <GLShaderType T>
bool GLShader<T>::create(const char* source)

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
        SKY_ERROR("OpenGL", "Failed to compile shader with id %d.\n%s", id, info_log);
        return false;
    }

    return true;
}

template <GLShaderType T>
void GLShader<T>::destroy()
{
    if (id != 0) {
        SKY_GL_CHECK_ERROR(glDeleteShader(id));
        id = 0;
    }
}


} // namespace sky
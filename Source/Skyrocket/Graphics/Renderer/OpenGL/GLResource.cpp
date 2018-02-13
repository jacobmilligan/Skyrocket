//
//  GLResource.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 7/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Graphics/Renderer/OpenGL/GLResource.hpp"

namespace sky {


const char* get_shader_type_string(const GLShaderType type)
{
    switch (type) {
        case GLShaderType::vertex: return "vertex";
        case GLShaderType::fragment: return "fragment";
        case GLShaderType::geometry: return "geometry";
        case GLShaderType::unknown: break;
    }

    return "unknown shader type";
}

void GLProgram::get_uniform_info(const uint32_t index, GLUniformInfo* info)
{
    info->program = id;
    info->index = index;

    // Get name and location
    SKY_GL_CHECK_ERROR(glGetActiveUniformName(id, index, GLUniformInfo::max_name,
                                              &info->name_len, info->name));
    SKY_GL_CHECK_ERROR(info->location = glGetUniformLocation(id, info->name));

    GLint type;
    SKY_GL_CHECK_ERROR(glGetActiveUniformsiv(id, 1, &index, GL_UNIFORM_SIZE, &info->size));
    SKY_GL_CHECK_ERROR(glGetActiveUniformsiv(id, 1, &index, GL_UNIFORM_TYPE, &type));
    info->type = gl_translate_uniform_type(type);

    SKY_GL_CHECK_ERROR(glGetActiveUniformsiv(id, 1, &index, GL_UNIFORM_BLOCK_INDEX,
                                             &info->block_index));
    SKY_GL_CHECK_ERROR(glGetActiveUniformsiv(id, 1, &index, GL_UNIFORM_OFFSET,
                                             &info->block_offset));
    SKY_GL_CHECK_ERROR(glGetActiveUniformsiv(id, 1, &index, GL_UNIFORM_ARRAY_STRIDE,
                                             &info->array_stride));
    SKY_GL_CHECK_ERROR(glGetActiveUniformsiv(id, 1, &index, GL_UNIFORM_MATRIX_STRIDE,
                                             &info->matrix_stride));
}

bool GLProgram::create(const char* vertex_source, const char* fragment_source)
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
        SKY_ERROR("OpenGL", "Failed to link shaders for program with id %d\n%s", id, info_log);
        return false;
    }

    SKY_GL_CHECK_ERROR(glDetachShader(id, vertex.id));
    SKY_GL_CHECK_ERROR(glDetachShader(id, fragment.id));

    vertex.destroy();
    fragment.destroy();

    SKY_GL_CHECK_ERROR(glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &num_uniforms));
    SKY_GL_CHECK_ERROR(glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &num_attrs));

    if (num_uniforms > 0 || num_attrs > 0) {
        GLUniformInfo info{};

        for (GLuint u = 0; u < num_uniforms; ++u) {
            get_uniform_info(u, &info);
            uniforms.push_back(info);
        }

        GLenum type;
        for (GLuint attr = 0; attr < num_attrs; ++attr) {
            SKY_GL_CHECK_ERROR(glGetActiveAttrib(id, attr, GLUniformInfo::max_name,
                                                 &info.name_len, &info.size, &type, info.name));
            if (strncmp(info.name, "sky_instance__", 14) == 0) {
                info.type = gl_translate_uniform_type(type);
                instances.push_back(info);
            }
        }
    }

    return true;
}

void GLProgram::destroy()
{
    if (id != 0) {
        SKY_GL_CHECK_ERROR(glDeleteShader(id));
        id = 0;
    }
}


} // namespace sky
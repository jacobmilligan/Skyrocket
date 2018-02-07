//
//  GLConfig.HPP
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 6/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Core/Config.hpp"
#import "Skyrocket/Graphics/Renderer/Definitions.hpp"

#include <OpenGL/gl3.h>

namespace sky {

inline const char* gl_get_enum_name(GLenum glenum)
{
    switch (glenum) {
#define SKY_GL_ENUM(enum_val) case enum_val: return #enum_val;

        SKY_GL_ENUM(GL_NO_ERROR)
        SKY_GL_ENUM(GL_INVALID_ENUM)
        SKY_GL_ENUM(GL_INVALID_VALUE)
        SKY_GL_ENUM(GL_INVALID_OPERATION)
        SKY_GL_ENUM(GL_INVALID_FRAMEBUFFER_OPERATION)
        SKY_GL_ENUM(GL_OUT_OF_MEMORY)

#if defined(__gl_h_) && !defined(__gl3_h_)
        SKY_GL_ENUM(GL_STACK_OVERFLOW)
        SKY_GL_ENUM(GL_STACK_UNDERFLOW)
        SKY_GL_ENUM(GL_TABLE_TOO_LARGE)
#endif

#undef SKY_GL_ENUM
        default: break;
    }

    return "Unknown GLenum value";
}

#define SKY_GL_CHECK_ERROR(fn) \
    SKY_BEGIN_MACRO_BLOCK \
        fn; \
        GLenum err = glGetError(); \
        while (err != GL_NO_ERROR) { \
            SKY_ERROR("OpenGL", "%s with call to: %s", sky::gl_get_enum_name(err), #fn); \
            SKY_DEBUG_BREAK(); \
            err = glGetError(); \
        } \
    SKY_END_MACRO_BLOCK

inline UniformType gl_translate_uniform_type(const GLint gltype)
{
    switch (gltype) {

        case GL_FLOAT:
        case GL_DOUBLE:
        case GL_INT:
        case GL_UNSIGNED_INT:
        case GL_BOOL: return UniformType::vec1;

        case GL_FLOAT_VEC2:
        case GL_DOUBLE_VEC2:
        case GL_INT_VEC2:
        case GL_UNSIGNED_INT_VEC2:
        case GL_BOOL_VEC2: return UniformType::vec2;

        case GL_FLOAT_VEC3:
        case GL_DOUBLE_VEC3:
        case GL_INT_VEC3:
        case GL_UNSIGNED_INT_VEC3:
        case GL_BOOL_VEC3: return UniformType::vec3;

        case GL_FLOAT_VEC4:
        case GL_DOUBLE_VEC4:
        case GL_INT_VEC4:
        case GL_UNSIGNED_INT_VEC4:
        case GL_BOOL_VEC4: return UniformType::vec4;

        case GL_FLOAT_MAT2:
        case GL_DOUBLE_MAT2: return UniformType::mat2;

        case GL_FLOAT_MAT3:
        case GL_DOUBLE_MAT3: return UniformType::mat3;

        case GL_FLOAT_MAT4:
        case GL_DOUBLE_MAT4: return UniformType::mat4;

        case GL_UNSIGNED_INT_SAMPLER_1D:
        case GL_INT_SAMPLER_1D:
        case GL_SAMPLER_1D: return UniformType::tex1d;

        case GL_UNSIGNED_INT_SAMPLER_2D:
        case GL_INT_SAMPLER_2D:
        case GL_SAMPLER_2D: return UniformType::tex2d;

        case GL_UNSIGNED_INT_SAMPLER_3D:
        case GL_INT_SAMPLER_3D:
        case GL_SAMPLER_3D: return UniformType::tex3d;

        case GL_SAMPLER_CUBE: return UniformType::cubemap;

//        case GL_FLOAT_MAT2x3:
//        case GL_FLOAT_MAT2x4:
//        case GL_FLOAT_MAT3x2:
//        case GL_FLOAT_MAT3x4:
//        case GL_FLOAT_MAT4x2:
//        case GL_FLOAT_MAT4x3:
//        case GL_DOUBLE_MAT2x3:
//        case GL_DOUBLE_MAT2x4:
//        case GL_DOUBLE_MAT3x2:
//        case GL_DOUBLE_MAT3x4:
//        case GL_DOUBLE_MAT4x2:
//        case GL_DOUBLE_MAT4x3:
//        case GL_SAMPLER_1D_SHADOW:
//        case GL_SAMPLER_2D_SHADOW:
//        case GL_SAMPLER_1D_ARRAY:
//        case GL_SAMPLER_2D_ARRAY:
//        case GL_SAMPLER_1D_ARRAY_SHADOW:
//        case GL_SAMPLER_2D_ARRAY_SHADOW:
//        case GL_SAMPLER_2D_MULTISAMPLE:
//        case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
//        case GL_SAMPLER_CUBE_SHADOW:
//        case GL_SAMPLER_BUFFER:
//        case GL_SAMPLER_2D_RECT:
//        case GL_SAMPLER_2D_RECT_SHADOW:
//        case GL_INT_SAMPLER_CUBE:
//        case GL_INT_SAMPLER_1D_ARRAY:
//        case GL_INT_SAMPLER_2D_ARRAY:
//        case GL_INT_SAMPLER_2D_MULTISAMPLE:
//        case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
//        case GL_INT_SAMPLER_BUFFER:
//        case GL_INT_SAMPLER_2D_RECT:
//        case GL_UNSIGNED_INT_SAMPLER_CUBE:
//        case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
//        case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
//        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
//        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
//        case GL_UNSIGNED_INT_SAMPLER_BUFFER:
//        case GL_UNSIGNED_INT_SAMPLER_2D_RECT:

        default:
            return UniformType::unknown;
    }
}

}



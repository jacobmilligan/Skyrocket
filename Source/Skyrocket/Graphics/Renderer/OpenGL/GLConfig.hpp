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


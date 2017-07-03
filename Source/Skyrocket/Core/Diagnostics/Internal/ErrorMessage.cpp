//
// Error.cpp
// Skyrocket Framework
//
// ----------------------------------------------------------------------------
//
// Created by Jacob Milligan on 10/12/2016.
// Copyright (c) 2016 Jacob Milligan All rights reserved.
//

#include "Skyrocket/Core/Diagnostics/ErrorMessage.hpp"

namespace sky {
namespace impl {

void __sky_print_error(const char* func, const char* file, const int line,
                 const char* type, const char* msgformat, ...)
{
    fprintf(stderr, "Skyrocket | %s[%s:%d] Error[%s]: ", func, file, line, type);
    
    va_list args;
    va_start(args, msgformat);
    vfprintf(stderr, msgformat, args);
    va_end(args);
    
    fprintf(stderr, "\n");
}
    
    
}
}

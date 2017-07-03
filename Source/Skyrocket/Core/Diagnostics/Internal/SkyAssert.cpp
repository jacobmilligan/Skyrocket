//
//  SkyAssert.cpp
//  Skyrocket Framework
//
//  --------------------------------------------------------------
//
//  Created by Jacob Milligan on 27/12/2016.
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Core/Diagnostics/SkyAssert.hpp"

namespace sky {
namespace impl {

[[noreturn]]
void __assert_handler(const char* function, const char* file, const int line,
                      const char* expr, const char* msgformat, ...)
{
    va_list args;
    va_start(args, msgformat);
    
    fprintf(stderr, "Skyrocket | Assertion failed: (%s): '",
           expr);
    vfprintf(stderr, msgformat, args);
    fprintf(stderr, "' [File:%s:%d][Function:%s]",
           file, line, function);
    fprintf(stderr, "\n");
    
    va_end(args);
    
    std::abort();
}


}
}

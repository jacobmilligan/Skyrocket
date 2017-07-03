//
//  SkyAssert.hpp
//  Skyrocket Framework
//
//  --------------------------------------------------------------
//
//  Created by Jacob Milligan on 11/12/2016.
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once


#include "Skyrocket/Platform/Config.hpp"

#include <cstdio>
#include <stdarg.h>
#include <cstdlib>

namespace sky {
namespace impl {

/// @brief Handles the assertion macro. Prints the format assertion string to stderr
/// @param function The function the assertion occurred in
/// @param file The file the assertion occurred in
/// @param line The line the assertion occurred in
/// @param expr String representing the assertion that failed
/// @param msgformat The message format string
/// @param ... Format arguments
[[noreturn]]
void __assert_handler(const char* function, const char* file, const int line,
                    const char* expr, const char* msgformat, ...);

}

#ifdef SKYROCKET_DEBUG
    
    /// @brief Writes a formatted string to stderr if the assertion expr is false
    /// @param expr The boolean expression to evaluate
    /// @param msgformat The message format string to display if the assertion
    /// returns false
    /// @param ... The messages format arguments
    #define SKY_ASSERT(expr, msgformat, ...)\
    {\
        if(!(expr)) {\
            sky::impl::__assert_handler(SKYROCKET_FUNCTION_NAME, __FILE__, __LINE__, \
                                        #expr, msgformat, ##__VA_ARGS__);\
        }\
    }

#else
    #define SKY_ASSERT(expr, msgformat, ...)
#endif


}

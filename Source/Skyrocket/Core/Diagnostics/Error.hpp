//
// Error.hpp
// Skyrocket Framework
//
// ----------------------------------------------------------------------------
//
// Created by Jacob Milligan on 7/12/2016.
// Copyright (c) 2016 Jacob Milligan All rights reserved.
//

#pragma once

#include "Skyrocket/Platform/Config.hpp"

#include <vector>

#include <cstdio>
#include <cstdlib>
#include <cstdarg>

namespace sky {

namespace impl {


/// @brief Prints a Skyrocket-formatted error message to the console
/// @param type The type of error that occurred
/// @param msgformat The format string for the message
/// @param ... Format parameters
void __sky_print_error(const char* func, const char* file, const int line,
                 const char* type, const char* msgformat, ...);

/// @brief Handles the assertion macro. Prints the format assertion string to stderr
/// @param function The function the assertion occurred in
/// @param file The file the assertion occurred in
/// @param line The line the assertion occurred in
/// @param expr String representing the assertion that failed
/// @param msgformat The message format string
/// @param ... Format arguments
void __sky_assert_handler(const char* function, const char* file, const int line,
                      const char* expr, const char* msgformat, ...);

} // impl


#if SKYROCKET_DEBUG
/// @brief Writes a formatted string to stderr if the assertion expr is false
/// @param expr The boolean expression to evaluate
/// @param msgformat The message format string to display if the assertion
/// returns false
/// @param ... The messages format arguments
#define SKY_ASSERT(expr, msgformat, ...)\
    {\
        if(!(expr)) {\
            sky::impl::__sky_assert_handler(SKYROCKET_FUNCTION_NAME, __FILE__, __LINE__, \
                                        #expr, msgformat, ##__VA_ARGS__);\
        }\
    }

#else
#define SKY_ASSERT(expr, msgformat, ...)
#endif

/// @brief Prints an error message with the line and file it occurred on
/// @param msg Error message to print
#define SKY_ERROR(type, msg, ...) sky::impl::__sky_print_error(SKYROCKET_FUNCTION_NAME, \
                                                    __FILE__, __LINE__, type, msg, ##__VA_ARGS__);

class AssertGuard {
public:
    AssertGuard(const char* action, const char* data);
    ~AssertGuard();
};


}

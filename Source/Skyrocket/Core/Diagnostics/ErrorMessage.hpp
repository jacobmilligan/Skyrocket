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

#include <cstdio>
#include <stdarg.h>
#include <cstdlib>

namespace sky {
namespace impl {

/// @brief Prints a Skyrocket-formatted error message to the console
/// @param type The type of error that occurred
/// @param msgformat The format string for the message
/// @param ... Format parameters
void __sky_print_error(const char* func, const char* file, const int line,
                 const char* type, const char* msgformat, ...);
    
}
}

/// @brief Prints an error message with the line and file it occurred on
/// @param msg Error message to print
#define SKY_ERROR(type, msg, ...) sky::impl::__sky_print_error(SKYROCKET_FUNCTION_NAME, \
                                                    __FILE__, __LINE__, type, msg, ##__VA_ARGS__);

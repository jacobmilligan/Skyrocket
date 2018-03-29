//
// Error.cpp
// Skyrocket Framework
//
// ----------------------------------------------------------------------------
//
// Created by Jacob Milligan on 10/12/2016.
// Copyright (c) 2016 Jacob Milligan All rights reserved.
//

#include "Skyrocket/Core/Diagnostics/Error.hpp"

namespace sky {
namespace detail {

struct AssertMessage {
    const char* action;
    const char* data;
};

thread_local std::vector<AssertMessage> assert_guards;

void __sky_assert_handler(const char* function, const char* file, const int line,
                          const char* expr, const char* msgformat, ...)
{
    fprintf(stderr, "--- Skyrocket ---\n");

    // print all asserts from oldest to newest
    for ( auto& msg : assert_guards ) {
        fprintf(stderr, "When %s: %s\n", msg.action, msg.data);
    }

    va_list args;
    va_start(args, msgformat);
    fprintf(stderr, "\tAssertion failed: (%s): ", expr);
    vfprintf(stderr, msgformat, args);
    fprintf(stderr, "\n\tAt File:%s:%d in Function: %s\n", file, line, function);
    va_end(args);

    assert_guards.clear();
}

void __sky_print_error(const char* func, const char* file, const int line,
                       const char* type, const char* msgformat, ...)
{
    fprintf(stderr, "--- Skyrocket ---\n%s error: ", type);

    va_list args;
    va_start(args, msgformat);

    vfprintf(stderr, msgformat, args);

    va_end(args);

    fprintf(stderr, "\nAt: %s:%d in: %s\n", file, line, func);
}


} // namespace detail


#if SKY_DEBUG == 1

AssertGuard::AssertGuard(const char* action, const char* data)
{
    detail::assert_guards.push_back({action, data});
}

AssertGuard::~AssertGuard()
{
    if ( !detail::assert_guards.empty() ) {
        detail::assert_guards.pop_back();
    }
}


#else

AssertGuard::AssertGuard(const char* action, const char* data) {}
AssertGuard::~AssertGuard() {}

#endif


} // namespace sky

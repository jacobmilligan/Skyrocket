//
//  Time.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 27/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Platform/Time.hpp"

#include <chrono>

#if SKY_OS_MACOS == 1

#include <mach/mach_time.h>

#endif

namespace sky {

uint64_t get_hertz() noexcept;

uint64_t high_resolution_time() noexcept
{
    auto count = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    return static_cast<uint64_t>(count);
}


uint64_t get_ticks_per_second() noexcept
{
    static const uint64_t hertz = get_hertz();

    return hertz;
}

uint64_t get_hertz() noexcept
{
    uint64_t hertz = 0;

#if SKY_OS_MACOS == 1

    mach_timebase_info_data_t ns_tick {};
    mach_timebase_info(&ns_tick);

    hertz = static_cast<uint64_t>(ns_tick.denom * 1e9 / ns_tick.numer);

#elif SKY_OS_WINDOWS == 1

    // Not implemented

#elif SKY_OS_LINUX == 1

    // Not implemented

#endif

    return hertz;
}


} // namespace sky
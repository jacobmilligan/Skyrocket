//
//  PlatformTime.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 1/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Platform/Platform.hpp"

#include <chrono>

namespace sky {


uint64_t high_resolution_time()
{
    auto count = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
    return static_cast<uint64_t>(count);
}


}

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

#include "Time.hpp"

#include <chrono>

namespace sky {


Time get_current_time()
{
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto nano = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();
    return Time(static_cast<uint64_t>(nano));
}


}

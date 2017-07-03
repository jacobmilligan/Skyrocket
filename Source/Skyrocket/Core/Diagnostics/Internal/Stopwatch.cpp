//
//  Stopwatch.cpp
//  Skyrocket Framework
//
//  --------------------------------------------------------------
//
//  Created by Jacob Milligan on 13/12/2016.
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Core/Diagnostics/Stopwatch.hpp"
#include "Skyrocket/Platform/Cross/Time.hpp"

namespace sky {


Stopwatch::Stopwatch()
    : time_(0) {}

void Stopwatch::start()
{
    time_ = sky::get_current_time();
}


Time Stopwatch::restart()
{
    auto result = elapsed();
    
    time_ = sky::get_current_time();
    
    return result;
}

Time Stopwatch::elapsed()
{
    return Time(
        sky::get_current_time().ticks() - time_.ticks()
    );
}


}

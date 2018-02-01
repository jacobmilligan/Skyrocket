//
//  Stopwatch.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 1/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Core/Diagnostics/Timespan.hpp"
#include "Skyrocket/Platform/Time.hpp"

namespace sky {


class Stopwatch {
public:
    Stopwatch()
        : stopped_(true)
    {}

    void start()
    {
        stopped_ = false;
        time_.reset(high_resolution_time());
    }

    void stop()
    {
        time_.reset(high_resolution_time() - time_.ticks());
        stopped_ = true;
    }

    const Timespan get_time() const
    {
        if (stopped_) {
            return time_;
        }

        return Timespan(high_resolution_time() - time_.ticks());
    }
private:
    bool stopped_;
    Timespan time_;
};


} // namespace sky
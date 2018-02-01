//
//  Stopwatch.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 1/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include <Skyrocket/Core/Diagnostics/Stopwatch.hpp>
#include <Skyrocket/Platform/Thread.hpp>

#include "catch/catch.hpp"

TEST_CASE("Stopwatch works", "[timer]")
{
    sky::Stopwatch timer;
    timer.start();
    sky::thread_sleep(sky::get_timespan<sky::TimeInterval::milliseconds>(5));
    timer.stop();

    REQUIRE(timer.get_time().milliseconds() < 6);
}
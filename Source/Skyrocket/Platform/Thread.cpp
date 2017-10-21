//
//  Thread.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 22/08/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Core/Diagnostics/Timespan.hpp"
#include "Skyrocket/Platform/Thread.hpp"

#include <ctime>

namespace sky {


void Semaphore::wait()
{
    std::unique_lock<std::mutex> lock(mut_);
    cv_.wait(lock, [&]() {
        return count_ > 0;
    });
    --count_;
    if ( count_ < 0 ) {
        count_ = 0;
    }
}

void Semaphore::signal()
{
    std::unique_lock<std::mutex> lock(mut_);
    ++count_;
    if ( count_ > max_count_ ) {
        count_ = max_count_;
    }
    cv_.notify_one();
}

void thread_sleep(const Timespan& time)
{
    static struct timespec sleep_time;
    static struct timespec remaining;

    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = static_cast<int64_t>(time.ticks());

    while ( sleep_time.tv_sec + sleep_time.tv_nsec > 0 ) {
        nanosleep(&sleep_time, &remaining);

        sleep_time.tv_sec = remaining.tv_sec;
        sleep_time.tv_nsec = remaining.tv_nsec;
    }
}


} // namespace sky
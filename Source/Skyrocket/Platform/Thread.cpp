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
#include "Skyrocket/Platform/Time.hpp"

#include <thread>

namespace sky {


void Semaphore::wait()
{
    std::unique_lock<std::mutex> lock(mut_);
    cv_.wait(lock, [&]() {
        return count_ > 0;
    });
    --count_;
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

void thread_sleep(const uint64_t ticks)
{
#if SKY_OS_MACOS == 1
    static struct timespec sleep_time;
    static struct timespec remaining;

    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = static_cast<long>(ticks);

    while ( nanosleep(&sleep_time, &remaining) == 1 && errno == EINTR ) {
        sleep_time.tv_sec = remaining.tv_sec;
        sleep_time.tv_nsec = remaining.tv_nsec;
    }
#else
    std::this_thread::sleep_for(std::chrono::nanoseconds(ticks));
#endif
}


} // namespace sky
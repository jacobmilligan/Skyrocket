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

constexpr uint64_t to_seconds(const uint64_t ticks) noexcept
{
    return ticks / Timespan::ticks_per_second;
}

constexpr uint64_t get_remaining_ticks(const uint64_t ticks) noexcept
{
    return ticks % Timespan::ticks_per_second;
}

void thread_sleep(const uint64_t ticks)
{
#if SKY_OS_MACOS == 1
    static struct timespec sleep_time{};

    sleep_time.tv_sec = static_cast<__darwin_time_t>(to_seconds(ticks));
    sleep_time.tv_nsec = static_cast<long>(get_remaining_ticks(ticks));

    nanosleep(&sleep_time, nullptr);
#else
    std::this_thread::sleep_for(std::chrono::nanoseconds(ticks));
#endif
}


} // namespace sky
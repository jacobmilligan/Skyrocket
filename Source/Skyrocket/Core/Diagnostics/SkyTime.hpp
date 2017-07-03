//
//  SkyTime.hpp
//  Skyrocket Framework
//
//  --------------------------------------------------------------
//
//  Created by Jacob Milligan on 13/12/2016.
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include <cstdint>

namespace sky {

/// @brief Time is a wrapper around a ticks value gained from the OS's
/// high-resolution clock (using sky::get_current_time()) or by specifying the time
/// value manually. It's also a utility structure for providing different interval
/// units and conversions between them
struct Time {
    
    /// @brief The number of ticks that make a millisecond
    static constexpr uint64_t ticks_per_millisecond = 1000;
    
    /// @brief The number of ticks that make a second
    static constexpr uint64_t ticks_per_second = 1000000;
    
    /// @brief The number of ticks that make a minute
    static constexpr uint64_t ticks_per_minute = ticks_per_second * 60;
    
    /// @brief The number of ticks that make an hour
    static constexpr uint64_t ticks_per_hour = ticks_per_minute * 60;
    
    /// @brief Represents an interval unit for representing the time
    /// value, used for conversions and timespan functions
    enum class Interval {
        ticks,
        milliseconds,
        seconds,
        minutes,
        hours
    };
    
    /// @brief Initializes the time structure using the specified hours, minutes,
    /// and seconds
    /// @param hours Number of hours in the time unit
    /// @param minutes Number of minutes in the time unit
    /// @param seconds Number of seconds in the time unit
    Time(const double hours, const double minutes,
                   const double seconds)
    {
        ticks_ = static_cast<uint64_t>(
            (hours * 3600 + minutes * 60 + seconds) * ticks_per_second
        );
    }
    
    /// @brief Initializes the time structure using the specified number of ticks
    /// @param ticks Number of ticks in the time unit
    Time(const uint64_t ticks)
        : ticks_(ticks) {}
    
    /// @brief Gets the time value in ticks
    /// @return The time in ticks
    inline uint64_t ticks() const
    {
        return ticks_;
    }
    
    /// @brief Gets the time value in milliseconds
    /// @return The time in milliseconds
	inline uint64_t milliseconds() const
    {
        return static_cast<uint64_t>(ticks_ * ms_per_tick_);
    }
    
    /// @brief Gets the time value in seconds
    /// @return The time in seconds
	inline double seconds() const
    {
        return ticks_ * secs_per_tick_;
    }
    
    /// @brief Gets the time value in minutes
    /// @return The time in minutes
	inline double minutes() const
    {
        return ticks_ * mins_per_tick_;
    }
    
    /// @brief Gets the time value in hours
    /// @return The time in hours
	inline double hours() const
    {
        return ticks_ * hours_per_tick_;
    }
    
private:
    /// @brief The time units ticks value used in all conversions
    uint64_t ticks_;
    
    /// @brief The fraction of milliseconds that make up a tick
    static constexpr double ms_per_tick_ = 1.0f / ticks_per_millisecond;
    
    /// @brief The fraction of seconds that make up a tick
    static constexpr double secs_per_tick_ = 1.0f / ticks_per_second;
    
    /// @brief The fraction of minutes that make up a tick
    static constexpr double mins_per_tick_ = 1.0f / ticks_per_minute;
    
    /// @brief The fraction of hours that make up a tick
    static constexpr double hours_per_tick_ = 1.0f / ticks_per_hour;
};

/// @brief Gets a new Time unit from a specified interval
/// @tparam T The Interval unit to use (ticks, seconds, minutes etc.)
/// @param duration The duration in the specified Interval to assign to the Time unit
/// @return The Time unit
template <Time::Interval T>
Time get_timespan(const double duration);

/// @brief Gets a new Time unit from a specified interval. Overload for working with
/// integer durations
/// @tparam T The Interval unit to use (ticks, seconds, minutes etc.)
/// @param duration The duration in the specified Interval to assign to the Time unit
/// @return The Time unit
template <Time::Interval T>
Time get_timespan(const uint64_t duration);

}

#include "Skyrocket/Core/Diagnostics/Internal/SkyTime.inl"

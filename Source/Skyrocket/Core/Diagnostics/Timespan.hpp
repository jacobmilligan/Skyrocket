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


/// @brief Represents an interval unit for representing the time
/// value, used for conversions and timespan functions
enum class TimeInterval {
    ticks,
    milliseconds,
    seconds,
    minutes,
    hours
};

/// @brief Time is a wrapper around a ticks value gained from the OS's
/// high-resolution clock (using sky::get_current_time()) or by specifying the time
/// value manually. It's also a utility structure for providing different interval
/// units and conversions between them
struct Timespan {

    /// @brief The number of ticks that make a microsecond
    static constexpr uint64_t ticks_per_microsecond = 10;

    /// @brief The number of ticks that make a millisecond
    static constexpr uint64_t ticks_per_millisecond = 10000;
    
    /// @brief The number of ticks that make a second
    static constexpr uint64_t ticks_per_second = 10000000;
    
    /// @brief The number of ticks that make a minute
    static constexpr uint64_t ticks_per_minute = ticks_per_second * 60;
    
    /// @brief The number of ticks that make an hour
    static constexpr uint64_t ticks_per_hour = ticks_per_minute * 60;

    Timespan() = default;

    /// @brief Initializes the time structure using the specified number of ticks
    /// @param ticks Number of ticks in the time unit
    explicit Timespan(const uint64_t ticks)
        : ticks_(ticks)
    {}
    
    /// @brief Initializes the time structure using the specified hours, minutes,
    /// and seconds
    /// @param hours Number of hours in the time unit
    /// @param minutes Number of minutes in the time unit
    /// @param seconds Number of seconds in the time unit
    Timespan(const double hours, const double minutes,
             const double seconds)
    {
        ticks_ = static_cast<uint64_t>(
            (hours * 3600 + minutes * 60 + seconds) * ticks_per_second
        );
    }
    
    /// @brief Gets the time value in ticks
    /// @return The time in ticks
    inline uint64_t ticks() const
    {
        return ticks_;
    }

    inline uint64_t microseconds() const
    {
        return static_cast<uint64_t>(ticks_ * micros_per_tick_);
    }
    
    /// @brief Gets the time value in milliseconds
    /// @return The time in milliseconds
	inline uint64_t milliseconds() const
    {
        return static_cast<uint64_t>(ticks_ * ms_per_tick_);
    }
    
    /// @brief Gets the time value in seconds
    /// @return The time in seconds
	inline uint64_t seconds() const
    {
        return static_cast<uint64_t>(ticks_ * secs_per_tick_);
    }
    
    /// @brief Gets the time value in minutes
    /// @return The time in minutes
	inline uint64_t minutes() const
    {
        return static_cast<uint64_t>(ticks_ * mins_per_tick_);
    }
    
    /// @brief Gets the time value in hours
    /// @return The time in hours
	inline uint64_t hours() const
    {
        return static_cast<uint64_t>(ticks_ * hours_per_tick_);
    }

    inline double total_microseconds() const
    {
        return ticks_ * micros_per_tick_;
    }

    /// @brief Gets the time value in milliseconds
    /// @return The time in milliseconds
    inline double total_milliseconds() const
    {
        return ticks_ * ms_per_tick_;
    }

    /// @brief Gets the time value in seconds
    /// @return The time in seconds
    inline double total_seconds() const
    {
        return ticks_ * secs_per_tick_;
    }

    /// @brief Gets the time value in minutes
    /// @return The time in minutes
    inline double total_minutes() const
    {
        return ticks_ * mins_per_tick_;
    }

    /// @brief Gets the time value in hours
    /// @return The time in hours
    inline double total_hours() const
    {
        return ticks_ * hours_per_tick_;
    }

    Timespan operator-(const Timespan& other) const
    {
        return Timespan(ticks_ - other.ticks_);
    }

    Timespan operator+(const Timespan& other) const
    {
        return Timespan(ticks_ + other.ticks_);
    }

    Timespan operator*(const Timespan& other) const
    {
        return Timespan(ticks_ * other.ticks_);
    }

    Timespan operator/(const Timespan& other) const
    {
        return Timespan(ticks_ / other.ticks_);
    }

    Timespan& operator-=(const Timespan& other)
    {
        ticks_ -= other.ticks_;
        return *this;
    }

    Timespan& operator+=(const Timespan& other)
    {
        ticks_ += other.ticks_;
        return *this;
    }

    Timespan& operator*=(const Timespan& other)
    {
        ticks_ *= other.ticks_;
        return *this;
    }

    Timespan& operator/=(const Timespan& other)
    {
        ticks_ /= other.ticks_;
        return *this;
    }

    bool operator==(const Timespan& other)
    {
        return ticks_ == other.ticks_;
    }

    bool operator!=(const Timespan& other)
    {
        return ticks_ != other.ticks_;
    }

    bool operator>(const Timespan& other)
    {
        return ticks_ > other.ticks_;
    }

    bool operator<(const Timespan& other)
    {
        return ticks_ < other.ticks_;
    }

    bool operator>=(const Timespan& other)
    {
        return ticks_ >= other.ticks_;
    }

    bool operator<=(const Timespan& other)
    {
        return ticks_ <= other.ticks_;
    }
    
private:
    /// @brief The time units ticks value used in all conversions
    uint64_t ticks_{0};

    /// @brief The fraction of milliseconds that make up a tick
    static constexpr double micros_per_tick_ = 1.0f / ticks_per_microsecond;

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
template <TimeInterval T>
Timespan get_timespan(const double duration);


}

#include "Skyrocket/Core/Diagnostics/Internal/Timespan.inl"

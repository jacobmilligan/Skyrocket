//
//  SkyTime.inl
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by Jacob Milligan on 24/12/2016.
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

namespace sky {

/// @brief Gets a new Time unit from a specified interval.
/// Template specialization for Time::Interval::ticks
/// @param duration The amount of ticks to assign to the Time unit
/// @return The Time unit
template <> inline
Time get_timespan<Time::Interval::ticks>(const uint64_t duration)
{
    return Time(duration);
}

/// @brief Gets a new Time unit from a specified interval.
/// Template specialization for Time::Interval::milliseconds
/// @param duration The amount of milliseconds to assign to the Time unit
/// @return The Time unit
template <> inline
Time get_timespan<Time::Interval::milliseconds>(const double duration)
{
    return Time(static_cast<uint64_t>(Time::ticks_per_millisecond * duration));
}

/// @brief Gets a new Time unit from a specified interval.
/// Template specialization for Time::Interval::seconds
/// @param duration The amount of seconds to assign to the Time unit
/// @return The Time unit
template <> inline
Time get_timespan<Time::Interval::seconds>(const double duration)
{
    return Time(static_cast<uint64_t>(Time::ticks_per_second * duration));
}

/// @brief Gets a new Time unit from a specified interval.
/// Template specialization for Time::Interval::minutes
/// @param duration The amount of minutes to assign to the Time unit
/// @return The Time unit
template <> inline
Time get_timespan<Time::Interval::minutes>(const double duration)
{
    return Time(static_cast<uint64_t>(Time::ticks_per_minute * duration));
}

/// @brief Gets a new Time unit from a specified interval.
/// Template specialization for Time::Interval::hours
/// @param duration The amount of hours to assign to the Time unit
/// @return The Time unit
template <> inline
Time get_timespan<Time::Interval::hours>(const double duration)
{
    return Time(static_cast<uint64_t>(Time::ticks_per_hour * duration));
}

}

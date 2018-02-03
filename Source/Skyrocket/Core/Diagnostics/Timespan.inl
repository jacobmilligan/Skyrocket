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


template<>
inline Timespan get_timespan<TimeInterval::ticks>(const double ticks)
{
    return Timespan(static_cast<uint64_t>(ticks));
}

template<>
inline Timespan get_timespan<TimeInterval::microseconds>(const double micros)
{
    return Timespan(static_cast<uint64_t>(Timespan::ticks_per_microsecond * micros));
}

/// @brief Gets a new Time unit from a specified interval.
/// Template specialization for Time::Interval::milliseconds
/// @param duration The amount of milliseconds to assign to the Time unit
/// @return The Time unit
template<>
inline Timespan get_timespan<TimeInterval::milliseconds>(const double millis)
{
    return Timespan(static_cast<uint64_t>(Timespan::ticks_per_millisecond * millis));
}

/// @brief Gets a new Time unit from a specified interval.
/// Template specialization for Time::Interval::seconds
/// @param duration The amount of seconds to assign to the Time unit
/// @return The Time unit
template<>
inline Timespan get_timespan<TimeInterval::seconds>(const double secs)
{
    return Timespan(static_cast<uint64_t>(Timespan::ticks_per_second * secs));
}

/// @brief Gets a new Time unit from a specified interval.
/// Template specialization for Time::Interval::minutes
/// @param duration The amount of minutes to assign to the Time unit
/// @return The Time unit
template<>
inline Timespan get_timespan<TimeInterval::minutes>(const double mins)
{
    return Timespan(static_cast<uint64_t>(Timespan::ticks_per_minute * mins));
}

/// @brief Gets a new Time unit from a specified interval.
/// Template specialization for Time::Interval::hours
/// @param duration The amount of hours to assign to the Time unit
/// @return The Time unit
template<>
inline Timespan get_timespan<TimeInterval::hours>(const double hours)
{
    return Timespan(static_cast<uint64_t>(Timespan::ticks_per_hour * hours));
}


} // namepsace sky

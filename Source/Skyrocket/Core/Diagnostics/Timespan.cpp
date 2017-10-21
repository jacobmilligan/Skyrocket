//
//  Timespan.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 27/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Core/Diagnostics/Timespan.hpp"
#include "Skyrocket/Platform/Time.hpp"

namespace sky {


/// @brief The number of ticks that make a microsecond
const uint64_t Timespan::ticks_per_microsecond = sky::get_ticks_per_second() / 1000000;

/// @brief The number of ticks that make a millisecond
const uint64_t Timespan::ticks_per_millisecond = sky::get_ticks_per_second() / 1000;

/// @brief The number of ticks that make a second
const uint64_t Timespan::ticks_per_second = get_ticks_per_second();

/// @brief The number of ticks that make a minute
const uint64_t Timespan::ticks_per_minute = get_ticks_per_second() / 60;

/// @brief The number of ticks that make an hour
const uint64_t Timespan::ticks_per_hour = (get_ticks_per_second() / 60) / 60;

/// @brief The fraction of milliseconds that make up a tick
const double Timespan::micros_per_tick_ = 1.0 / static_cast<double>(ticks_per_microsecond);

/// @brief The fraction of milliseconds that make up a tick
const double Timespan::ms_per_tick_ = 1.0 / static_cast<double>(ticks_per_millisecond);

/// @brief The fraction of seconds that make up a tick
const double Timespan::secs_per_tick_ = 1.0 / static_cast<double>(ticks_per_second);

/// @brief The fraction of minutes that make up a tick
const double Timespan::mins_per_tick_ = 1.0 / static_cast<double>(ticks_per_minute);

/// @brief The fraction of hours that make up a tick
const double Timespan::hours_per_tick_ = 1.0 / static_cast<double>(ticks_per_hour);


} // namespace sky
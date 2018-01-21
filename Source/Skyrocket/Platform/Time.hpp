//
//  Time.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 15/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Core/Config.hpp"

#include <cstdint>

namespace sky {

/// @brief Gets the current high resolution time in ticks
/// @return
uint64_t high_resolution_time() noexcept;

/// @brief Gets the number of ticks that occur for every second for the current platform
/// @return
uint64_t get_ticks_per_second() noexcept;


} // namespace sky
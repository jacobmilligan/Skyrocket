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

#include "Skyrocket/Platform/Config.hpp"

#include <cstdint>

namespace sky {


uint64_t high_resolution_time() noexcept;

uint64_t get_ticks_per_second() noexcept;


} // namespace sky
//
//  PlatformTime.hpp
//  Skyrocket Framework
//
//  --------------------------------------------------------------
//
//  Created by Jacob Milligan on 13/12/2016.
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Core/Diagnostics/SkyTime.hpp"

namespace sky {

/// @brief Gets the current time epoch for the current platform since it was
/// turned on
/// @return Time since the current platform was turned on
Time get_current_time();

}

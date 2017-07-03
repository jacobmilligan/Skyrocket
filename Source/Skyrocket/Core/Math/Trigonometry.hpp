//
//  Trigonometry.hpp
//  Skyrocket Framework
//
//  --------------------------------------------------------------
//
//  Created by Jacob Milligan on 12/02/2017.
//  Copyright (c) 2017 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Common.hpp"

namespace sky {
namespace math {

/// @brief Converts an angle from degrees to radians
/// @param angle Angle in degrees to convert
/// @return The angle in radians
constexpr double to_radians(const double degrees)
{
    return degrees * (pi / 180.0);
}

/// @brief Converts an angle in radians to degrees
/// @param radians Angle to convert
/// @return The angle in degrees
constexpr double to_degrees(const double radians)
{
    return radians * (180.0 / pi);
}
    
}
}
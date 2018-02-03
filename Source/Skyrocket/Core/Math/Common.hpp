//
//  Common.hpp
//  Skyrocket Framework
//
//  --------------------------------------------------------------
//
//  Created by Jacob Milligan on 19/12/2016.
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Core/Config.hpp"

namespace sky {
namespace math {

/// @brief Constant representing the number Pi to a precision of 20 decimal points
static constexpr double pi = 3.14159265358979323846;

/// @brief Clamps a value to stay within a specified minimum and maximum
/// works on any type with defined or overloaded operator > or operator <
/// @tparam T The data type of the numeric value
/// @param value The value to clamp
/// @param low The lower bound of the clamping operation
/// @param high The upper bound of the clamping operation
/// @return The clamped value
template<typename T>
SKY_FORCE_INLINE T clamp(const T& value, const T& low, const T& high)
{
    return value < low ? low : (value > high ? high : value);
}


}  // namespace math
}  // namespace sky

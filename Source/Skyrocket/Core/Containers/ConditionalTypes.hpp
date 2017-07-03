//
//  ConditionalTypes.hpp
//  Skyrocket Framework
//
//  --------------------------------------------------------------
//
//  Created by Jacob Milligan on 25/02/2017.
//  Copyright (c) 2017 Jacob Milligan. All rights reserved.
//

#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace sky {

/// @defgroup ConditionalTypes A collection of types that change depending on defined
/// conditions, an example being the conditional_uint which one of uint8, uint16, or uin32
/// depending on the expected maximum value held within. These are mainly used internally
/// within Skyrocket for efficient usage of space for indices and id values in containers.
/// @{

/// @brief Represents a conditional unsigned integer with a minimum value of uint8 and
/// a maximum value of uint32
template <size_t Size>
using conditional_uint = typename std::conditional<Size <= UINT8_MAX, uint8_t,
    typename std::conditional<Size <= UINT16_MAX, uint16_t,uint32_t
    >::type
>::type;

/// @brief Represents a large conditional unsigned integer with a minimum value of uint16
/// and a maximum value of uint64
template <size_t Size>
using large_conditional_uint = typename std::conditional<Size <= UINT16_MAX, uint16_t,
    typename std::conditional<Size <= UINT32_MAX, uint32_t, uint64_t
    >::type
>::type;

/// @}

}
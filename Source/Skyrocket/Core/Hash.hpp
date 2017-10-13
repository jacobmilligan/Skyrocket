//
//  Hash.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 13/10/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include <cstdint>

namespace sky {
namespace hash {


uint32_t murmur3_32(const void* key, uint32_t len, uint32_t seed);


} // namespace hash
} // namespace sky
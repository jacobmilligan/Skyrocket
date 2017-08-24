//
//  Memory.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 21/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include <cstdint>

namespace sky {

/// @brief Contains a block of memory and its size
struct MemoryBlock {
    /// @brief The byte size of the memory block
    uint32_t size{0};
    /// @brief Pointer to the data being stored
    void* data{nullptr};
};


} // namespace sky

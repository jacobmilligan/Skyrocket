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
#include <cstddef>

namespace sky {

/// @brief Contains a block of memory and its size
struct MemoryBlock {
    /// @brief The byte size of the memory block
    uint32_t size{0};
    /// @brief Pointer to the data being stored
    void* data{nullptr};
};

constexpr uint64_t kibibytes(const uint64_t amount)
{
    return 1024 * amount;
}

constexpr uint64_t mebibytes(const uint64_t amount)
{
    return (1024 * 1024) * amount;
}

constexpr uint64_t gibibytes(const uint64_t amount)
{
    return (1024 * 1024 * 1024) * amount;
}

inline bool is_aligned(const void* ptr, const size_t alignment) noexcept
{
    return (reinterpret_cast<uintptr_t>(ptr) % alignment) == 0;
}

inline bool is_power_of_two(size_t num)
{
    return (num && (num & (num - 1)) == 0) || num == 1;
}

MemoryBlock alloc(uint32_t size, void* data);

void* align(void* ptr, size_t alignment);


} // namespace sky

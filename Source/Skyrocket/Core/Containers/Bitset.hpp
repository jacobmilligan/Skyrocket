//
//  Bitset.HPP
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 3/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include <Skyrocket/Core/Config.hpp>

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace sky {

template <size_t Size>
struct Bitset {
    static constexpr uint32_t data_size = 32 - 1;
    using array_type_t = uint32_t[Size];

    array_type_t bits{};

    SKY_FORCE_INLINE Bitset()
    {
        clear_all();
    }

    SKY_FORCE_INLINE constexpr size_t byte_position(const size_t bit) const noexcept
    {
        return bit >> 5; // log2(32)
    }

    SKY_FORCE_INLINE bool is_set(const size_t bit) const noexcept
    {
        return (bits[byte_position(bit)] & (1 << (bit & data_size))) != 0;
    }

    SKY_FORCE_INLINE void set_bit(const size_t bit) noexcept
    {
        bits[byte_position(bit)] |= 1 << (bit & data_size);
    }

    SKY_FORCE_INLINE void clear_bit(const size_t bit) noexcept
    {
        bits[byte_position(bit)] &= ~(1 << (bit & data_size));
    }

    SKY_FORCE_INLINE void toggle_bit(const size_t bit) noexcept
    {
        bits[byte_position(bit)] ^= 1 << (bit & data_size);
    }

    SKY_FORCE_INLINE void set_all() noexcept
    {
        memset(bits, UINT32_MAX, Size);
    }

    SKY_FORCE_INLINE void clear_all() noexcept
    {
        memset(bits, 0, Size);
    }
};


} // namespace sky
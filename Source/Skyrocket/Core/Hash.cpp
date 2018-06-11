//
//  Hash.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 13/10/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Core/Hash.hpp"
#include "Config.hpp"

#if SKY_COMPILER_MSVC == 1

#include <stdlib.h>

#define ROTL32(x,y)	_rotl(x,y)
#define ROTL64(x,y)	_rotl64(x,y)

#define BIG_CONSTANT(x) (x)

#else

inline uint32_t rotl32 ( uint32_t x, int8_t r )
{
    return (x << r) | (x >> (32 - r));
}

inline uint64_t rotl64 ( uint64_t x, int8_t r )
{
    return (x << r) | (x >> (64 - r));
}

#define	ROTL32(x,y)	rotl32(x,y)
#define ROTL64(x,y)	rotl64(x,y)

#define BIG_CONSTANT(x) (x##LLU)

#endif

namespace sky {
namespace hash {

/**
 * Finalization mix - force all bits of a hash block to avalanche
 * @param h
 * @return
 */
SKY_FORCE_INLINE uint32_t fmix32 (uint32_t h)
{
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

uint32_t murmur3_32(const void* key, uint32_t len, uint32_t seed)
{
    static const uint32_t c1 = 0xcc9e2d51;
    static const uint32_t c2 = 0x1b873593;

    const auto* data = static_cast<const uint8_t*>(key);
    const int nblocks = len / 4;

    uint32_t h1 = seed;
    const auto* blocks = reinterpret_cast<const uint32_t*>(data + nblocks * 4);

    uint32_t k1 = 0;
    for ( int i = -nblocks; i != 0; ++i ) {
        k1 = blocks[i];

        k1 *= c1;
        k1 = ROTL32(k1,15);
        k1 *= c2;

        h1 ^= k1;
        h1 = ROTL32(h1,13);
        h1 = h1 * 5 + 0xe6546b64;
    }

    const auto* tail = data + nblocks * 4;
    k1 = 0;

    switch (len & 3) {
        case 3: k1 ^= tail[2] << 16;
        case 2: k1 ^= tail[1] << 8;
        case 1:
        {
            k1 ^= tail[0] << 16;
            k1 *= c1;
            k1 = ROTL32(k1, 15);
            k1 *= c2;
            h1 ^= k1;
        }
        default: break;
    }

    h1 ^= len;
    h1 = fmix32(h1);

    return h1;
}


} // namespace hash
} // namespace sky

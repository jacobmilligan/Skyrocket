//
//  Memory.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 28/10/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Memory.hpp"

#include <cstdlib>
#include <cstring>

namespace sky {


MemoryBlock alloc(uint32_t size, void* data)
{
    MemoryBlock mb;

    mb.size = size;
    mb.data = malloc(size);
    memcpy(mb.data, data, size);

    return mb;
}

void* align(void* ptr, size_t alignment)
{
    const auto intptr = reinterpret_cast<uintptr_t>(ptr);
    return reinterpret_cast<void*>((intptr + alignment - 1) & -alignment);
}


} // namespace sky
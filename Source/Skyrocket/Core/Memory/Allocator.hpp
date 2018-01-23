//
//  Allocator.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 21/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include <cstddef>

namespace sky {


class Allocator {
public:
    virtual void* allocate(size_t byte_size, size_t alignment) = 0;

    void* allocate(size_t byte_size)
    {
        return allocate(byte_size, 1);
    }

    virtual void free(void* ptr) = 0;
    virtual void reset() = 0;
    virtual bool is_valid(void* ptr) const = 0;
};


} // namespace sky
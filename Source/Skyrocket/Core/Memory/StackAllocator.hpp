//
//  StackAllocator.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 21/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Core/Diagnostics/Error.hpp"
#include "Skyrocket/Core/Memory/Memory.hpp"
#include "Skyrocket/Core/Memory/Allocator.hpp"

namespace sky {


class FixedStackAllocator : public Allocator {
public:
    using Allocator::allocate;

    explicit FixedStackAllocator(const size_t capacity)
        : cursor_(0), capacity_(capacity), buffer_(nullptr)
    {
        buffer_ = new uint8_t[capacity_];
        reset();
    }

    ~FixedStackAllocator()
    {
        delete [] buffer_;
    }

    void* allocate(const size_t byte_size, const size_t alignment) override
    {
        if (cursor_ + byte_size >= capacity_) {
            SKY_ERROR("FixedStackAllocator",
                      "Unable to allocate more memory than the allocator has available");
            return nullptr;
        }

        if (!is_power_of_two(alignment)) {
            SKY_ERROR("FixedStackAllocator", "Alignment must be a power of two");
            return nullptr;
        }

        auto ptr = align(&buffer_[cursor_], alignment);
        cursor_ = (static_cast<uint8_t*>(ptr) - buffer_) + byte_size;
        return ptr;
    }

    void free(void* ptr) override
    {
        // No op for stack allocator
        SKY_ERROR("FixedStackAllocator", "Stack allocators disallow freeing memory ad-hoc. "
            "All frees must be stack-based and done via `free_to_cursor`");
    }

    void free_to_cursor(const size_t cursor)
    {
        if (cursor >= cursor_) {
            SKY_ERROR("FixedStackAllocator",
                      "Cannot free to cursor (%zu) larger than the allocators current "
                          "size (%zu) or capacity (%zu)",
                      cursor, cursor_, capacity_);
            return;
        }

        cursor_ = cursor;
        memset(buffer_ + cursor_, 0, capacity_ - cursor_);
    }

    void reset() override
    {
        free_to_cursor(0);
    }

    bool is_valid(void* ptr) const override
    {
        return ptr >= buffer_ && ptr <= buffer_ + capacity_;
    }

    inline size_t cursor() const
    {
        return cursor_;
    }

private:
    size_t cursor_;
    size_t capacity_;
    uint8_t* buffer_;
};


} // namespace sky
//
//  FixedPoolAllocator.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 24/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Core/Memory/Allocator.hpp"
#include "Skyrocket/Core/Diagnostics/Error.hpp"

#include <cstdint>

namespace sky {


// TODO(Jacob): Implement alignment

/// A pool of fixed-size that allocates raw memory in same-sized blocks. All memory managed by the
/// allocator is created at construction and deleted at destruction.
class FixedPoolAllocator : public Allocator {
public:
    using Allocator::allocate;

    FixedPoolAllocator() = default;

    /// Initializes the allocator with `block_size` bytes allocated per memory block and
    /// `max_blocks` number of blocks capacity
    FixedPoolAllocator(const size_t block_size, const uint32_t max_blocks)
    {
        SKY_ASSERT(block_size >= sizeof(void*), "`block_size` is larger than sizeof void* (%lu)",
                   sizeof(void*));

        block_size_ = block_size;
        max_blocks_ = max_blocks;
        memory_ = new uint8_t[block_size_ * max_blocks_];

        reset();
    }

    FixedPoolAllocator(const FixedPoolAllocator& other) = delete;
    FixedPoolAllocator& operator=(const FixedPoolAllocator& other) = delete;

    FixedPoolAllocator(FixedPoolAllocator&& other) noexcept
    {
        std::swap(memory_, other.memory_);
        std::swap(next_, other.next_);
        std::swap(block_size_, other.block_size_);
        std::swap(max_blocks_, other.max_blocks_);
        std::swap(blocks_initialized_, other.blocks_initialized_);
    }

    FixedPoolAllocator& operator=(FixedPoolAllocator&& other) noexcept
    {
        std::swap(memory_, other.memory_);
        std::swap(next_, other.next_);
        std::swap(block_size_, other.block_size_);
        std::swap(max_blocks_, other.max_blocks_);
        std::swap(blocks_initialized_, other.blocks_initialized_);

        return *this;
    }

    ~FixedPoolAllocator()
    {
        delete[] memory_;
    }

    void* allocate(size_t byte_size, size_t alignment) override
    {
        // all blocks are of fixed size
        if ( blocks_initialized_ == max_blocks_ ) {
            return nullptr;
        }

        ++blocks_initialized_;

        uint8_t* block = next_;
        next_ = reinterpret_cast<uint8_t**>(next_)[0];
        memset(block, 0, block_size_);
        return block;
    }

    /// Frees memory allocated by `allocate()`. If the data wasn't allocated by the internal pool,
    /// this function causes undefined behaviour and will log an error
    void free(void* ptr) override
    {
        if ( ptr == nullptr ) {
            SKY_ERROR("FixedPoolAllocator", "tried to free a null pointer");
            return;
        }

        if ( blocks_initialized_ <= 0 ) {
            SKY_ERROR("FixedPoolAllocator",
                      "tried to free more blocks than the pool has available");
            return;
        }

        if ( !is_valid(ptr) ) {
            SKY_ERROR("FixedPoolAllocator",
                      "attempted to free memory that wasn't allocated by the pool");
            return;
        }

        reinterpret_cast<uint8_t**>(ptr)[0] = next_;
        next_ = static_cast<uint8_t*>(ptr);
        --blocks_initialized_;
    }

    /// Resets the pool and zeroes out all memory
    void reset() override
    {
        next_ = memory_;
        blocks_initialized_ = 0;

        uint8_t* ptr = &memory_[0];
        uint8_t* last = &memory_[block_size_ * max_blocks_];
        while ( ptr < last ) {
            new (ptr) uint8_t*(ptr + block_size_);

            ptr += block_size_;
        }
    }

    bool is_valid(void* ptr) const override
    {
        return ptr >= memory_ && ptr < memory_ + (block_size_ * max_blocks_);
    }

    /// Gets the maximum number of blocks the pool can allocate
    inline uint32_t block_capacity() const
    {
        return max_blocks_;
    }

    /// Gets the current number of blocks allocated
    inline uint32_t blocks_initialized() const
    {
        return blocks_initialized_;
    }
private:
    uint8_t* memory_{nullptr};
    uint8_t* next_{nullptr};

    size_t block_size_{0};
    uint32_t max_blocks_{0};

    uint32_t blocks_initialized_{0};
};


}
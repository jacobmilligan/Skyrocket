//
//  Buffer.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 26/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

// TODO(Jacob): Implement ring buffer

#pragma once

#include "Skyrocket/Core/Diagnostics/Error.hpp"

#include <cstdint>

namespace sky {


template <uint32_t Size>
class Buffer {
public:
    Buffer() = default;

    template <typename T>
    void write(T* data)
    {
        auto size = sizeof(T);

        SKY_ASSERT(cursor_ + size < Size, "sizeof T (%lu) is small enough to fit in buffer", size);

        memcpy(&data_[cursor_], data, size);
        cursor_ += size;
        end_ = cursor_;
    }

    template <typename T>
    T* read()
    {
        auto size = sizeof(T);

        SKY_ASSERT(cursor_ + size < Size,
                   "sizeof T (%lu) is small enough to prevent reading past end of buffer",
                   size);

        auto mem = static_cast<void*>(&data_[cursor_]);
        cursor_ += size;
        return static_cast<T*>(mem);
    }

    template <typename T>
    void read_copy(T* out_data)
    {
        auto size = sizeof(T);
        SKY_ASSERT(cursor_ + size < Size,
                   "sizeof T (%lu) is small enough to prevent reading past end of buffer",
                   size);

        memcpy(out_data, &data_[cursor_], size);
        cursor_ += size;
    }

    void reset()
    {
        cursor_ = 0;
    }

    void clear()
    {
        cursor_ = 0;
        end_ = 0;
    }

    const uint32_t& end() const
    {
        return end_;
    }

    const uint32_t& cursor_pos() const
    {
        return cursor_;
    }

    uint32_t size() const
    {
        return Size;
    }

private:
    uint32_t end_{0};
    uint32_t cursor_{0};
    uint8_t data_[Size]{};
};

template <uint32_t Size, uint16_t NumBuffers>
class CommandBuffer {
public:
    CommandBuffer() = default;

    void flip()
    {
        write_buf_ = static_cast<uint16_t>(write_buf_ + 1) % NumBuffers;
        read_buf_ = static_cast<uint16_t>(read_buf_ + 1) % NumBuffers;

        buffers_[write_buf_].clear();
        buffers_[read_buf_].reset();
    }

    Buffer<Size>& get_read()
    {
        return buffers_[read_buf_];
    }

    Buffer<Size>& get_write()
    {
        return buffers_[write_buf_];
    }

    uint16_t read_index()
    {
        return read_buf_;
    }

    uint16_t write_index()
    {
        return write_buf_;
    }

private:
    uint16_t read_buf_{0};
    uint16_t write_buf_{1};
    Buffer<Size> buffers_[NumBuffers];
};


} // namespace sky
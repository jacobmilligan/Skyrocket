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

/// @brief A simple wrapper around a buffer of data of varying size and type - read and
/// write operations handle moving the internal cursor and end position.
/// @tparam Size Number of bytes to store in the buffer
template <uint32_t Size>
class Buffer {
public:
    Buffer() = default;

    /// @brief Writes data of any type to the buffer via copy
    /// @tparam T The type of data being written allows for specifying byte size
    /// @param data The data to write to the buffer
    template <typename T>
    void write(T* data)
    {
        auto size = sizeof(T);

        SKY_ASSERT(cursor_ + size < Size, "sizeof T (%lu) is small enough to fit in buffer", size);

        memcpy(&data_[cursor_], data, size);
        cursor_ += size;
        end_ = cursor_;
    }

    /// @brief Reads data from the buffer, copying it into the return pointer
    /// @tparam T Type of data being read. This allows the current element to be read
    /// into any receiving pointer type.
    /// @return Pointer to the copied data
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

    /// @brief Reads data from the buffer, copying it into the specified pointer
    /// @tparam T The type of data being read
    /// @param out_data The pointer to copy the data into
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

    /// @brief Resets the cursor position to the start of the buffer
    void reset_cursor()
    {
        cursor_ = 0;
    }

    /// @brief Clears the buffer, resetting the cursor and end positions
    void clear()
    {
        cursor_ = 0;
        end_ = 0;
    }

    /// @brief Gets the position of the last active byte in the buffer
    /// @return
    const uint32_t& end() const
    {
        return end_;
    }

    /// @brief Gets the cursors current byte index
    /// @return The cursor position
    const uint32_t& cursor_pos() const
    {
        return cursor_;
    }

    /// @brief Gets the total available size of the buffer
    /// @return The size of the buffer
    uint32_t size() const
    {
        return Size;
    }

private:
    uint32_t end_{0};
    uint32_t cursor_{0};
    uint8_t data_[Size]{};
};

/// @brief A simple multi-buffer that allows for switching buffers for multi-threading
/// operations
/// @tparam Size Number of bytes to store in each buffer
/// @tparam NumBuffers The number of buffer available
template <uint32_t Size, uint16_t NumBuffers>
class Multibuffer {
public:
    Multibuffer() = default;

    /// @brief Flips the buffers
    void flip()
    {
        write_buf_ = static_cast<uint16_t>(write_buf_ + 1) % NumBuffers;
        read_buf_ = static_cast<uint16_t>(read_buf_ + 1) % NumBuffers;

        buffers_[write_buf_].clear();
        buffers_[read_buf_].reset_cursor();
    }

    /// @brief Gets a reference to the current read buffer
    /// @return Reference to the read buffer
    Buffer<Size>& get_read()
    {
        return buffers_[read_buf_];
    }

    /// @brief Gets a reference to the current write buffer
    /// @return Reference to the write buffer
    Buffer<Size>& get_write()
    {
        return buffers_[write_buf_];
    }

    /// @brief Gets the index of the current read buffer
    /// @return Index of the read buffer
    uint16_t read_index()
    {
        return read_buf_;
    }

    /// @brief Gets the index of the current write buffer
    /// @return Index of the write buffer
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
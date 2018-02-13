//
//  CommandList.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 23/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Graphics/Renderer/CommandData.hpp"

#include <cstdint>
#include <array>

namespace sky {

struct CommandBuffer {
    static constexpr auto buffer_max = kibibytes(512);

    uint8_t buffer[buffer_max]{};

    void clear()
    {
        cursor_ = 0;
        size_ = 0;
        memset(buffer, 0, buffer_max);
    }

    inline void reset_cursor()
    {
        cursor_ = 0;
    }

    inline size_t cursor() const
    {
        return cursor_;
    }

    inline size_t size() const
    {
        return size_;
    }

    template <typename T>
    T* read_command()
    {
        if (cursor_ + sizeof(T) > buffer_max) {
            return nullptr;
        }

        auto src = reinterpret_cast<T*>(&buffer[cursor_]);
        cursor_ += sizeof(T);
        return src;
    }

    template <typename Command>
    bool write_command(const CommandType type, const Command& cmd)
    {
        if (cursor_ + sizeof(Command) + sizeof(CommandType) > buffer_max) {
            SKY_ERROR("CommandList", "Attempted to write to full command buffer");
            return false;
        }

        // Make sure cursor is read and incremented before any other threads see the change
        auto c = cursor_;
        cursor_ += sizeof(Command) + sizeof(CommandType);
        size_ = cursor_;

        std::atomic_thread_fence(std::memory_order_release);

        buffer[c] = static_cast<uint8_t>(type);
        ++c;
        memcpy(buffer + c, &cmd, sizeof(Command));

        return true;
    }

    bool write_command(const CommandType type)
    {
        if (cursor_ + sizeof(CommandType) > buffer_max) {
            SKY_ERROR("CommandList", "Attempted to write to full command buffer");
            return false;
        }

        // Make sure cursor is read and incremented before any other threads see the change
        auto c = cursor_;
        cursor_ += sizeof(CommandType);
        size_ = cursor_;

        std::atomic_thread_fence(std::memory_order_release);

        buffer[c] = static_cast<uint8_t>(type);

        return true;
    }
private:
    size_t cursor_{0};
    size_t size_{0};
};

class CommandList {
public:
    uint64_t sort_key{0};
    CommandBuffer* buffer{nullptr};

    CommandList() = default;

    explicit CommandList(CommandBuffer* cmdbuffer)
        : buffer(cmdbuffer)
    {
        clear();
    }

    /// Reads the number of bytes equal to sizeof(T) into `dest` and increments the internal cursor
    template <typename T>
    T* read()
    {
        return buffer->read_command<T>();
    }

    inline void clear()
    {
        buffer->clear();
    }

    inline void reset_cursor()
    {
        buffer->reset_cursor();
    }

    inline size_t cursor() const
    {
        return buffer->cursor();
    }

    inline size_t size() const
    {
        return buffer->size();
    }

    void set_viewport(Viewport* viewport);

    void set_clear_color(const Color& color);

    /// @brief Sends a command to create a new vertex buffer resource
    /// @param initial_data The initial data to copy into the buffer
    /// @param usage
    /// @return Handle id to the vertex buffer resource
    uint32_t create_vertex_buffer(const MemoryBlock& initial_data,
                                  BufferUsage usage);

    /// @brief Sets the vertex buffer associated with the specified id as the current one
    /// @param vbuf_id
    /// @param offset
    /// @param num_vertices
    void set_vertex_buffer(uint32_t vbuf_id, uint32_t offset,
                           uint32_t num_vertices);

    void update_vertex_buffer(uint32_t vbuf_id, const MemoryBlock& data);

    /// @brief Sends a command to create a new index buffer resource
    /// @param initial_data The initial data to copy into the buffer
    /// @return Handle id to the index buffer resource
    uint32_t create_index_buffer(const MemoryBlock& initial_data);

    /// @brief Sets the index buffer associated with the specified id as the current one
    /// @param ibuf_id
    /// @param offset
    /// @param num_indices
    void set_index_buffer(uint32_t ibuf_id, uint32_t offset,
                          uint32_t num_indices);

    /// @brief Sends a command to create a new shader uniform resource
    /// @param type The type of structure or data associated with the uniform
    /// @param count The number of instances of `type` associated with the uniform
    /// @return Handle id to the shader uniform resource
    uint32_t create_uniform(UniformType type, uint32_t size);

    /// @brief Sets the uniform associated with the given ID as the current one
    /// @param u_id
    /// @param index Index to assign to the uniform as referenced in the shader
    void set_uniform(uint32_t u_id, uint32_t index);

    /// @brief Sends a command to update the data associated with a shader uniform
    /// @param u_id
    /// @param data
    void update_uniform(uint32_t u_id, const MemoryBlock& data, uint32_t offset = 0);

    /// @brief Sends a command to create a new shader
    /// @param vs_path
    /// @return Handle ID to the shader
    uint32_t create_program(const Path& vs_path, const Path& frag_path);

    /// @brief Sets the vertex and fragment shaders as the current ones
    /// @param vertex_id
    /// @param fragment_id
    /// @return
    bool set_program(uint32_t program_id);

    uint32_t create_texture(uint32_t width, uint32_t height,
                            PixelFormat::Enum pixel_format, bool mipmapped = false);

    void create_texture_region(uint32_t texture, const UIntRect& region,
                               PixelFormat::Enum pixel_format, uint8_t* data);

    bool set_texture(uint32_t texture, uint32_t index);

    void set_state(uint32_t state_flags);

    /// @brief Sends a command to draw triangle primitives
    void draw();

    void draw_instanced(uint32_t instances);

private:
    static uint32_t next_handle_;

    inline uint32_t make_handle()
    {
        return next_handle_++;
    }
};


} // namespace sky
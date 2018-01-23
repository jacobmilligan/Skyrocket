//
//  CommandBuffer.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 23/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Core/Geometry/Rectangle.hpp"
#include "Skyrocket/Platform/Filesystem.hpp"
#include "Skyrocket/Core/Memory/Memory.hpp"
#include "Skyrocket/Graphics/GDI/Definitions.hpp"

#include <cstdint>
#include <array>

namespace sky {

class CommandBuffer;

enum class CommandType : uint8_t {
    unknown,
    init,
    set_viewport,
    create_vertex_buffer,
    create_index_buffer,
    create_program,
    create_uniform,
    create_texture,
    create_texture_region,
    set_vertex_buffer,
    set_index_buffer,
    set_program,
    set_uniform,
    set_texture,
    update_uniform,
    draw,
    draw_instanced,
    set_state
};

struct CreateVertexBufferData {
    uint32_t buf_id{0};
    MemoryBlock data;
    BufferUsage buf_usage{BufferUsage::none};
};

struct SetVertexBufferData {
    uint32_t buf_id{0};
    uint32_t first_vertex{0};
    uint32_t count{0};
};

struct CreateIndexBufferData {
    uint32_t buf_id{0};
    MemoryBlock data;
};

struct SetIndexBufferData {
    uint32_t buf_id{0};
    uint32_t first_index{0};
    uint32_t count{0};
};

struct CreateProgramData {
    uint32_t prog_id{0};
    char vs[SKY_MAX_PATH]{};
    char frag[SKY_MAX_PATH]{};
};

struct CreateUniformData {
    uint32_t uniform_id;
    UniformType uniform_type;
    uint32_t size;
};

struct SetUniformData {
    uint32_t uniform_id;
    uint32_t uniform_index;
};

struct UpdateUniformData {
    uint32_t uniform_id{0};
    uint32_t offset{0};
    MemoryBlock new_data;
};

struct CreateTextureData {
    uint32_t tid;
    uint32_t width;
    uint32_t height;
    PixelFormat::Enum format;
    bool mipmapped;
};

struct CreateTextureRegionData {
    uint32_t tex_id{0};
    UIntRect rect;
    PixelFormat::Enum format{PixelFormat::unknown};
    uint8_t* data{nullptr};
};

struct SetTextureData {
    uint32_t tid;
    uint32_t index;
};

class CommandBuffer {
public:
    enum class State {
        unknown,
        ready,
        recording,
        processing
    };

    void begin();

    void end();

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
    uint32_t create_uniform(UniformType type, uint16_t count);

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
    static constexpr auto buffer_capacity_ = kibibytes(64);

    State state_{State::ready};
    size_t cursor_{0};
    uint8_t buffer_[buffer_capacity_]{};

    static uint32_t next_handle_;

    inline uint32_t make_handle()
    {
        return (state_ == State::recording) ? next_handle_++ : 0;
    }

    template <typename Command>
    bool write_command(const CommandType type, const Command& cmd)
    {
        if (state_ != State::recording) {
            SKY_ERROR("CommandBuffer", "Buffer must be in the `recording` state in order "
                "to write commands, i.e. commands can only be written between "
                "`begin` and `end` calls");
            return false;
        }

        if (cursor_ + sizeof(Command) + sizeof(CommandType) > buffer_capacity_) {
            SKY_ERROR("CommandBuffer", "Attempted to write to full command buffer");
            return false;
        }

        // Make sure cursor is read and incremented before any other threads see the change
        auto c = cursor_;
        cursor_ += sizeof(Command) + sizeof(CommandType);

        std::atomic_thread_fence(std::memory_order_release);

        buffer_[c] = static_cast<uint8_t>(type);
        ++c;
        memcpy(buffer_ + c, &cmd, sizeof(Command));

        return true;
    }

    bool write_command(const CommandType type)
    {
        if (state_ != State::recording) {
            SKY_ERROR("CommandBuffer", "Buffer must be in the `recording` state in order "
                "to write commands, i.e. commands can only be written between "
                "`begin` and `end` calls");
            return false;
        }

        if (cursor_ + sizeof(CommandType) > buffer_capacity_) {
            SKY_ERROR("CommandBuffer", "Attempted to write to full command buffer");
            return false;
        }

        // Make sure cursor is read and incremented before any other threads see the change
        auto c = cursor_;
        cursor_ += sizeof(CommandType);

        std::atomic_thread_fence(std::memory_order_release);

        buffer_[c] = static_cast<uint8_t>(type);

        return true;
    }
};


} // namespace sky
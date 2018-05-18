//
//  GDI.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 23/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Graphics/Renderer/FrameInfo.hpp"
#include "Skyrocket/Core/Geometry/Rectangle.hpp"
#include "Skyrocket/Graphics/Renderer/Definitions.hpp"

#include <Shadecc.hpp>

namespace sky {

class Path;
class Viewport;
class CommandList;
class CommandBuffer;
class MemoryBlock;

/// Graphics API's supported by Skyrocket.
enum class RendererBackend {
    unknown,
    none,
    Metal,
    OpenGL,
    D3D9,
    D3D11,
    D3D12,
    Vulkan,
    last
};

/// Array type used for getting a list of supported renderer backends.
using renderer_backend_list_t = RendererBackend[static_cast<size_t>(RendererBackend::last)];

/// Populates the supplied list with a list of supported renderer backends ordered by the current
/// platforms preferred graphics API. For instance, on macOS, the list would contain:
/// {Metal, OpenGL, none, none, ...} but on Windows it would contain:
/// {D3D12, Vulkan, D3D11, OpenGL, D3D9, none, none, ...}.
void supported_renderer_backends(renderer_backend_list_t& dest);

/// Contains handles for the resources bound to the GPU for the current draw call - vertex buffers,
/// shaders, index buffers etc.
struct RenderState {
    RenderState()
    {
        reset();
    }

    /// @brief Resets all values back to zero
    void reset()
    {
        vertex_buffer = 0;
        vertex_count = 0;
        vertex_offset = 0;
        index_count = 0;
        index_buffer = 0;
        index_offset = 0;
    }

    uint32_t vertex_buffer{0};
    uint32_t vertex_count{0};
    uint32_t vertex_offset{0};
    uint32_t index_buffer{0};
    uint32_t index_count{0};
    uint32_t index_offset{0};
    uint32_t program{0};

//    struct UniformSlot {
//        uint32_t handle;
//        uint32_t index;
//    }
    uint32_t uniform_slots[1024]{};
    uint32_t instance_slots[1024]{};
};

/// Graphics Device Interface - an interface for executing calls to the currently
/// active renderer backend.
///
/// Each virtual function represents a call that must be implemented
/// by the backend-specific GDI in order to provide the full features required by the engine.
/// Each GDI implementation also manages all resources required by the renderer in an API-specific
/// way, releasing them all upon destruction. This means that when a graphics backend is changed,
/// or the GDI needs to be recreated for any particular reason, the user must recreate all
/// previously allocated resource handles by calling the appropriate creation commands -
/// `create_vertex_buffer`, `create_program` etc.
///
/// The base GDI is **not** an abstract interface but simply a base class that acts as a 'no-op'
/// backend that simply takes command buffers and then does nothing with them. This is useful for
/// situations where the graphics device is unavailable for any particular reason, but can still
/// take `CommandBuffer` submissions from the application to remove cognitive load on the user - to
/// them the scene just simply isn't rendering and they can query the GDI for it's type to see if
/// there's an issue, rather than making them avoid `CommandList` submissions.
class GDI {
public:
    static constexpr uint32_t invalid_handle = 0;
    static constexpr uint16_t vertex_buffer_max = 1024;
    static constexpr uint16_t index_buffer_max = 1024;
    static constexpr uint16_t uniform_buffer_max = 512;
    static constexpr uint16_t shader_max = 512;
    static constexpr uint16_t texture_max = 512;
    static constexpr uint16_t instance_buffer_max = 512;
    static constexpr uint16_t max_frames_in_flight = 3;

    GDI() = default;

    virtual ~GDI() = default;

    /// @brief Creates a new API-specific GDI. A GDI cannot be created without calling
    /// this method
    /// @return Unique pointer to the GDI
    static std::unique_ptr<GDI> create(RendererBackend backend = RendererBackend::unknown,
                                       GDI* copy = nullptr) noexcept;

    /// @brief Initializes the graphics device, allocating resources and creating a
    /// device context
    /// @param viewport The viewport to attach the graphics device to
    /// @return Successful initialization if true, false otherwise
    virtual bool init(Viewport* viewport);

    virtual bool destroy();

    /// Begins processing of a new frame - a series of `CommandBuffer` objects that contain
    /// render commands to be processed in the order the appear. Should only be called once per
    /// frame.
    virtual bool begin_frame(FrameInfo* frame_info);

    /// Ends the processing of the current frame and handles submission to the GPU. Should only be
    /// called once per frame.
    virtual bool end_frame(FrameInfo* frame_info);

    /// Submits a command buffer for translation into API-specific draw calls. Must be called
    /// between a `begin_frame` and `end_frame` pair. Can be called multiple times with different
    /// command buffers between the one begin and end calls.
    void submit(CommandBuffer* cmdbuf);

    /// @brief Sets the viewport as the active viewport for this graphics device
    /// @param viewport
    virtual void set_viewport(Viewport* viewport);

    virtual void set_clear_color(const Color& color);

    inline RendererBackend backend() const
    {
        return backend_;
    }
protected:
    RenderState state_;

    /// @brief Creates a new vertex buffer
    /// @param vbuf_id The id handle of the buffer to create
    /// @param initial_data The data to copy into the buffer
    /// @param usage
    /// @return Success if true, false otherwise
    virtual bool create_vertex_buffer(uint32_t vbuf_id, const MemoryBlock& initial_data,
                                      BufferUsage usage);

    /// @brief Sets the vertex buffer associated with the given id as the current one
    /// @param vbuf_id
    /// @return Success if true, false otherwise
    virtual bool set_vertex_buffer(uint32_t vbuf_id);

    virtual bool update_vertex_buffer(uint32_t vbuf_id, const MemoryBlock& data);

    /// @brief Creates a new index buffer
    /// @param ibuf_id The id handle of the buffer to create
    /// @param initial_data
    /// @return
    virtual bool create_index_buffer(uint32_t ibuf_id, const MemoryBlock& initial_data);

    /// @brief Sets the index buffer associated with the given id as the current one
    /// @param ibuf_id
    /// @return Success if true, false otherwise
    virtual bool set_index_buffer(uint32_t ibuf_id);

    /// @brief Draws primitive triangle data
    /// @return
    virtual bool draw();

    /// @brief Draws primitive triangle data
    /// @return
    virtual bool draw_instanced(uint32_t instance);

    /// @brief Creates a new shader
    /// @param program_id
    /// @param name
    /// @return
    virtual bool create_program(uint32_t program_id, const shadecc::ShaderSource& vs_src,
                                const shadecc::ShaderSource& fs_src);

    virtual bool set_program(uint32_t program_id);

    virtual bool create_uniform(uint32_t u_id, const char* name, uint32_t size, UniformType type);

    virtual bool set_uniform(uint32_t u_id, uint32_t index);

    virtual bool update_uniform(uint32_t u_id, const MemoryBlock& data, uint32_t offset);

    virtual bool create_instance_buffer(uint32_t inst_id, uint32_t stride, uint32_t size);

    virtual bool set_instance_buffer(uint32_t inst_id, uint32_t index);

    virtual bool update_instance_buffer(uint32_t inst_id, uint8_t* data, uint32_t index);

    virtual bool create_texture(uint32_t t_id, uint32_t width,
                                uint32_t height, PixelFormat pixel_format,
                                bool mipmapped);

    virtual bool create_texture_region(uint32_t tex_id, const UIntRect& region,
                                       PixelFormat pixel_format, uint8_t* data);

    virtual bool set_texture(uint32_t t_id, uint32_t index);

    virtual bool set_state(uint32_t flags);

private:
    RendererBackend backend_{RendererBackend::unknown};
};


} // namespace sky
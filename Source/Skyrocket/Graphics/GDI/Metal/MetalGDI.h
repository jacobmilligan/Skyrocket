//
//  MetalGDI.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 22/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

//TODO(Jacob): make some kind of metal buffer pool class for handling semaphores and multi-buffering

#pragma once

#include "Skyrocket/Core/Containers/HandleTable.hpp"
#include "Skyrocket/Graphics/GDI/GDI.hpp"

#import <AppKit/AppKit.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

#include <unordered_map>

namespace sky {

template<uint16_t Size>
struct MetalBuffer {
    MetalBuffer()
        :
        current_(0), usage_(BufferUsage::none)
    {}

    void init(id<MTLDevice> device, void* data, const uint32_t length, const BufferUsage usage)
    {
        current_ = 0;
        usage_ = usage;
        if ( data == nullptr ) {
            buf_ = [device newBufferWithLength:length
            options:MTLResourceCPUCacheModeDefaultCache];
        } else {
            buf_ = [device newBufferWithBytes:data
            length:length
            options:MTLResourceCPUCacheModeDefaultCache];
        }
    }

    id<MTLBuffer>& raw_buffer()
    {
        return buf_;
    }

private:
    id<MTLBuffer> buf_;
    uint16_t current_;
    BufferUsage usage_;
};

class MetalProgram {
public:
    MetalProgram() = default;
    MetalProgram(const uint32_t program_id, id<MTLFunction> vs, id<MTLFunction> frag);
    ~MetalProgram();

    id<MTLRenderPipelineState> get_render_pipeline_state(id<MTLDevice> device);
private:
    static constexpr uint32_t default_state_flags = MTLPixelFormatBGRA8Unorm;

    uint32_t program_id_;
    id<MTLFunction> vs_;
    id<MTLFunction> frag_;
    std::unordered_map<uint32_t, id<MTLRenderPipelineState>> render_pipeline_states_;
};


class MetalGDI : public GDI {
public:
    MetalGDI();

    ~MetalGDI() override;

    bool initialize(Viewport* viewport) override;

    void set_viewport(Viewport* viewport) override;

    bool create_vertex_buffer(const uint32_t vbuf_id, const MemoryBlock& initial_data,
                              const BufferUsage usage) override;

    bool set_vertex_buffer(const uint32_t vbuf_id) override;

    bool create_index_buffer(const uint32_t ibuf_id, const MemoryBlock& initial_data) override;

    bool set_index_buffer(const uint32_t ibuf_id) override;

    bool create_program(const uint32_t shader_id, const Path& vs_path, const Path& frag_path) override;

    bool set_program(const uint32_t program_id) override;

    bool create_uniform(const uint32_t u_id, const uint32_t size) override;

    void set_uniform(const uint32_t u_id, const uint32_t index) override;

    void update_uniform(const uint32_t u_id, const MemoryBlock& data, const uint32_t offset) override;

    void create_texture(const uint32_t t_id, const uint8_t* data, const int32_t width,
                        const int32_t height, const int32_t bytes_per_pixel,
                        const bool mipmapped) override;

    void set_texture(const uint32_t t_id, const uint32_t index) override;

    bool draw() override;

    bool draw_instanced(const uint32_t instance) override;

    void set_state(const uint32_t flags) override;

    void commit() override;

private:
    static constexpr uint8_t lib_max_ = 8;

    id<MTLDevice> device_;
    id<MTLCommandQueue> command_queue_;
    id<MTLRenderPipelineState> render_pipeline_;

    id<MTLCommandBuffer> command_buffer_[max_frames_in_flight];
    id<MTLRenderCommandEncoder> render_encoder_;

    id<MTLLibrary> default_library_;

    MetalProgram default_program_;

    id<MTLDepthStencilState> depth_stencil_state_;

    dispatch_semaphore_t buf_sem_;

    CAMetalLayer* mtl_layer_;

    HandleTable<MetalBuffer<max_frames_in_flight>, vertex_buffer_max> vertex_buffers_;
    HandleTable<MetalBuffer<max_frames_in_flight>, index_buffer_max> index_buffers_;
    HandleTable<MetalBuffer<max_frames_in_flight>, uniform_buffer_max> uniform_buffers_;
    HandleTable<MetalProgram, shader_max> programs_;
    HandleTable<id<MTLTexture>, texture_max> textures_;

    uint32_t buffer_index_{0};
};


}  // namespace sky

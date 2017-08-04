//
//  MetalDriver.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 22/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Graphics/GDI/GDI.hpp"
#include "Skyrocket/Core/Containers/HandleTable.hpp"

#import <AppKit/AppKit.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

namespace sky {


template <uint16_t Size>
struct MetalBuffer {
    MetalBuffer()
        : current_(0), usage_(BufferUsage::none)
    {}

    void init(id<MTLDevice> device, void* data, const uint32_t length, const BufferUsage usage)
    {
        current_ = 0;
        usage_ = usage;
        buffers_[0] = [device newBufferWithBytes:data length:length options:0];
    }

    void swap()
    {
        ++current_;
        if ( current_ >= Size ) {
            current_ = 0;
        }
    }

    id<MTLBuffer>& current()
    {
        return buffers_[current_];
    }

private:
    id<MTLBuffer> buffers_[Size];
    uint16_t current_;
    BufferUsage usage_;
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

    bool create_shader(const uint32_t shader_id, const char* name) override;

    bool set_shaders(const uint32_t vertex_id, const uint32_t fragment_id) override;

    void present() override;

private:
    id<MTLDevice> device_;
    id<MTLCommandQueue> command_queue_;
    id<MTLRenderPipelineState> render_pipeline_;

    id<MTLCommandBuffer> command_buffer_[max_frames_in_flight];
    id<MTLRenderCommandEncoder> render_encoder_;
    
    id<MTLLibrary> library_;
    id<MTLLibrary> default_library_;
    id<MTLFunction> default_vshader_;
    id<MTLFunction> default_fragshader_;
    
    dispatch_semaphore_t buf_sem_;

    CAMetalLayer* mtl_layer_;

    HandleTable<MetalBuffer<2>, vertex_buffer_max> vertex_buffers_;
    HandleTable<id<MTLFunction>, shader_max> shaders_;
};


}

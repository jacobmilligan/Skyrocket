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
        if ( current_ >= Size )
            current_ = 0;
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

    ~MetalGDI();

    bool initialize() override;

    void set_viewport(Viewport* viewport) override;

    uint32_t
    create_vertex_buffer(const MemoryBlock& initial_data, const BufferUsage usage) override;
    uint32_t create_shader() override;

    void present() override;

private:
    id<MTLDevice> device_;
    id<MTLCommandQueue> command_queue_;
    id<MTLRenderPipelineState> render_pipeline_;
    CAMetalLayer* mtl_layer_;

    HandleTable<MetalBuffer<2>, vertex_buffer_max> vertex_buffers_;
    HandleTable<id<MTLLibrary>, shader_max> shaders_;
};


}
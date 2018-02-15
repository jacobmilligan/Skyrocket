//
//  MetalResource.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 30/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Graphics/Renderer/Definitions.hpp"

#include <cstdint>
#include <unordered_map>

#import <Metal/Metal.h>

namespace sky {


template<uint16_t Size>
class MetalBuffer {
public:
    MetalBuffer()
        : current_(0), usage_(BufferUsage::none)
    {}

    void destroy()
    {
        for (int i = 0; i < Size; ++i) {
            if (buf_[i] != nil) {
                SKY_OBJC_RELEASE(buf_[i]);
            }
        }
    }

    void init(id <MTLDevice> device, void* data, const uint32_t length, const sky::BufferUsage usage)
    {
        options_ = MTLResourceCPUCacheModeDefaultCache;
        length_ = length;
        usage_ = usage;

        switch (usage_) {
            case BufferUsage::staticbuf:
            {
                options_ = MTLResourceStorageModeShared;
            } break;
            case BufferUsage::dynamic:
            {
                options_ = MTLResourceCPUCacheModeWriteCombined | MTLResourceStorageModeManaged;
            } break;
            default:
                break;
        }

        if ( data == nullptr ) {
            buf_[current_] = [device newBufferWithLength:length_
                                                 options:options_];
        } else {
            buf_[current_] = [device newBufferWithBytes:data
                                                 length:length_
                                                options:options_];
        }
    }

    void update(id <MTLDevice> device, void* data, const uint32_t size, const uint32_t offset = 0)
    {
        if (usage_ == BufferUsage::dynamic) {
            switch_buffers();
        }

        if (!buf_[current_]) {
            buf_[current_] = [device newBufferWithLength:length_
                                                 options:options_];
        }

        auto contents = static_cast<uint8_t*>([buf_[current_] contents]);

        if (usage_ == BufferUsage::staticbuf) {
            memset(contents, 0, length_);
        }
        
        memcpy(contents + offset, data, size);
        [buf_[current_] didModifyRange:NSMakeRange(offset, offset + size)];
    }

    inline id<MTLBuffer>& raw_buffer()
    {
        return buf_[current_];
    }

private:
    id<MTLBuffer> buf_[Size]{};
    uint32_t length_;
    uint16_t current_;
    BufferUsage usage_;
    MTLResourceOptions options_;

    inline void switch_buffers()
    {
        current_ = static_cast<uint16_t>((current_ + 1) % Size);
    }
};

template <uint16_t Size>
struct MetalInstanceBuffer {
    uint32_t stride;
    MetalBuffer<Size> buffer;
};

class MetalProgram {
public:
    MetalProgram() = default;
    MetalProgram(uint32_t program_id, id<MTLFunction> vs, id<MTLFunction> frag);

    void destroy();

    id<MTLRenderPipelineState> get_render_pipeline_state(id<MTLDevice> device);
private:
    static constexpr uint32_t default_state_flags = MTLPixelFormatBGRA8Unorm;

    uint32_t program_id_{0};
    id<MTLFunction> vs_{nil};
    id<MTLFunction> frag_{nil};
    std::unordered_map<uint32_t, id<MTLRenderPipelineState>> render_pipeline_states_;
};


} // namespace sky

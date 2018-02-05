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

#include "Skyrocket/Graphics/GDI/Definitions.hpp"

#include <cstdint>
#include <unordered_map>

#import <Metal/Metal.h>

#define SKY_OBJC_RELEASE(object) [object release], (object) = nil

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
        usage_ = usage;
        if ( data == nullptr ) {
            buf_[current_] = [device newBufferWithLength:length
                                                 options:MTLResourceCPUCacheModeDefaultCache];
        } else {
            buf_[current_] = [device newBufferWithBytes:data
                                                 length:length
                                                options:MTLResourceCPUCacheModeDefaultCache];
        }
    }

    void update(id <MTLDevice> device, void* data, const uint32_t size)
    {
        switch (usage_) {
            case BufferUsage::none:break;

            case BufferUsage::staticbuf:
            {
                SKY_OBJC_RELEASE(buf_[current_]);
                buf_[current_] = [device newBufferWithBytes:data
                                                     length:size
                                                    options:MTLResourceCPUCacheModeDefaultCache];
                memcpy([buf_[current_] contents], data, size);
            } break;

            case BufferUsage::dynamic:
            {
                switch_buffers();

                if (!buf_[current_]) {
                    buf_[current_] = [device newBufferWithBytes:data
                                                         length:size
                                                        options:MTLResourceCPUCacheModeDefaultCache];
                } else {
                    memcpy([buf_[current_] contents], data, size);
                }
            } break;
        }
    }

    inline id<MTLBuffer>& raw_buffer()
    {
        return buf_[current_];
    }

private:
    id<MTLBuffer> buf_[Size]{};
    uint16_t current_;
    BufferUsage usage_;

    inline void switch_buffers()
    {
        current_ = static_cast<uint16_t>((current_ + 1) % Size);
    }
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

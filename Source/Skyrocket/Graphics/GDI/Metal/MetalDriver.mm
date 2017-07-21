//
//  MetalDriver.mm
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 6/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Core/Containers/HandleTable.hpp"
#include "Skyrocket/Graphics/GDI/GDI.hpp"

#include <array>

#import <AppKit/AppKit.h>
#import <Metal/Metal.h>

namespace sky {

template <uint16_t Size>
struct MetalBuffer {
    MetalBuffer()
        : current_(0)
    {}

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
};

class MetalGDI : public GDI {
public:
    MetalGDI()
    {}

    ~MetalGDI()
    {

    }

    bool initialize() override
    {
        device = MTLCreateSystemDefaultDevice();
//        command_queue = [device newCommandQueue];
        return true;
    }

    VertexBufferHandle
    create_vertex_buffer(const uint32_t size, const BufferUsage usage,
                         const MemoryBlock& initial_data) override
    {
    }

private:
    id<MTLDevice> device;
    MetalBuffer<2> vertex_buffers_[vertex_buffer_max];
};

std::unique_ptr<GDI> create_graphics_device_interface()
{
    return std::make_unique<MetalGDI>();
}
    
}

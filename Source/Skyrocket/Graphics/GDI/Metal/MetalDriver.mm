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

#include "Skyrocket/Graphics/GDI/Metal/MetalDriver.h"
#include "Skyrocket/Graphics/Internal/Apple/MacViewport.h"
#include "Skyrocket/Graphics/Internal/Apple/MetalView.h"

#include <array>



namespace sky {

MetalGDI::MetalGDI()
    : GDI()
{}

MetalGDI::~MetalGDI()
{}

bool MetalGDI::initialize()
{
    device_ = MTLCreateSystemDefaultDevice();
    command_queue_ = [device_ newCommandQueue];
    
    MTLRenderPipelineDescriptor* pipeline_descriptor = [MTLRenderPipelineDescriptor new];
    pipeline_descriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;

    return true;
}

void MetalGDI::set_viewport(Viewport* viewport)
{
    MetalView* mtl_view = (MetalView*)viewport->get_native_viewport()->view;
    mtl_view.metalLayer.device = device_;
    mtl_layer_ = mtl_view.metalLayer;
}

uint32_t
MetalGDI::create_vertex_buffer(const MemoryBlock& initial_data, const BufferUsage usage)
{
    auto vbuf_id = vertex_buffers_.create();
    vertex_buffers_.lookup(vbuf_id)->init(device_, initial_data.data, initial_data.size, usage);
    return vbuf_id;
}

void MetalGDI::present()
{
    id<CAMetalDrawable> drawable = [mtl_layer_ nextDrawable];
    if ( drawable ) {
        
    }
}

uint32_t MetalGDI::create_shader(const char* path)
{
//    MTLLibrary
    return shaders_.invalid_id;
}

std::unique_ptr<GDI> create_graphics_device_interface()
{
    return std::make_unique<MetalGDI>();
}
    
}

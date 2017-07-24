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
#include "Skyrocket/Platform/Filesystem.hpp"

//TODO(Jacob): Program struct - vertex shader and fragment shader combination
//TODO(Jacob): Shader struct - either a fragment, vertex, or geometry shader
//TODO(Jacob): Render commands - simple POD with ids and an enum type and data* depending on the command (i.e. set_vertex_data will need a void*)
//TODO(Jacob): Textures - <sigh>

namespace sky {


MetalGDI::MetalGDI()
{}

MetalGDI::~MetalGDI() = default;

bool MetalGDI::initialize()
{
    device_ = MTLCreateSystemDefaultDevice();
    command_queue_ = [device_ newCommandQueue];
    
    MTLRenderPipelineDescriptor* pipeline_descriptor = [MTLRenderPipelineDescriptor new];
    pipeline_descriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
    
    Path lib_path(SKY_RESOURCE_DIRECTORY);
    lib_path.append("Bin/Shaders/Metal/Lib.metallib");
    
    NSError* err = nil;
    NSString* nspath = [NSString stringWithUTF8String:lib_path.str()];
    library_ = [device_ newLibraryWithFile:nspath error:&err];

    if ( library_ == nil ) {
        SKY_ERROR("Graphics Device Interface", "Couldn't load metal library with path %s: NSError: %s",
                  lib_path.str(), [[err localizedDescription] UTF8String]);
        return false;
    }
    
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
    if ( drawable != nil ) {
        
    }
}

uint32_t MetalGDI::create_shader(const char* name)
{
    NSString* nsname = [NSString stringWithUTF8String:name];
    id<MTLFunction> func = [library_ newFunctionWithName:nsname];
    
    if ( func == nil ) {
        SKY_ERROR("Shader", "Couldn't create shader with name %s", name);
        return shaders_.invalid_id;
    }
    
    return shaders_.create(func);
}

bool MetalGDI::set_shader(const uint32_t vertex_id, const uint32_t fragment_id)
{
    if ( !shaders_.valid(vertex_id) ) {
        SKY_ERROR("Shader", "%" PRIu32 " is an invalid vertex shader ID", vertex_id);
        return false;
    }
    if ( !shaders_.valid(fragment_id) ) {
        SKY_ERROR("Shader", "%" PRIu32 " is an invalid vertex shader ID", fragment_id);
        return false;
    }

}

std::unique_ptr<GDI> create_graphics_device_interface()
{
    return std::make_unique<MetalGDI>();
}
    
}  // namespace sky

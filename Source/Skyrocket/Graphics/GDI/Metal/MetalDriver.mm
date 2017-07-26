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

//TODO(Jacob): Textures

namespace sky {


std::unique_ptr<GDI> create_graphics_device_interface()
{
    return std::make_unique<MetalGDI>();
}

MetalGDI::MetalGDI() = default;

MetalGDI::~MetalGDI() = default;

bool MetalGDI::initialize(Viewport* viewport)
{
    //---------------------------------------
    //  Initialize device and command queue
    //---------------------------------------

    NSError* err = nil;
    device_ = MTLCreateSystemDefaultDevice();
    
    set_viewport(viewport);
    
    command_queue_ = [device_ newCommandQueue];
    command_buffer_ = [command_queue_ commandBufferWithUnretainedReferences];

    //--------------------------------
    //  Load library and shader path
    //--------------------------------
    Path lib_path(SKY_RESOURCE_DIRECTORY);
    lib_path.append("Shaders/Metal/Lib.metallib");

    NSString* nspath = [NSString stringWithUTF8String:lib_path.str()];
    library_ = [device_ newLibraryWithFile:nspath error:&err];

    if ( library_ == nil ) {
        SKY_ERROR("Graphics Device Interface", "Couldn't load metal library with path %s: NSError: %s",
                  lib_path.str(), [[err localizedDescription] UTF8String]);
        return false;
    }

    //--------------------------------
    //  Load default library
    //--------------------------------
    
    const char* default_src = R"(
#include <metal_stdlib>

using namespace metal;

struct Vertex {
    float4 position [[position]];
    float4 color;
};

vertex Vertex basic_vertex( device Vertex* vertices [[buffer(0)]], uint vid [[vertex_id]] )
{
    return vertices[vid];
}

fragment float4 basic_fragment(Vertex in [[stage_in]])
{
    return in.color;
}
    )";

    NSString* nssrc = [NSString stringWithUTF8String:default_src];
    
    default_library_ = [device_ newLibraryWithSource:nssrc options:nil error:&err];
    if ( default_library_ == nil ) {
        SKY_ERROR("Graphics Device Interface", "Couldn't load default metal library: NSError: %s",
                  lib_path.str(), [[err localizedDescription] UTF8String]);
        return false;
    }

    default_vshader_ = [default_library_ newFunctionWithName:@"basic_vertex"];
    default_fragshader_ = [default_library_ newFunctionWithName:@"basic_fragment"];

    //--------------------------------
    //  Load main render pipeline
    //--------------------------------

    MTLRenderPipelineDescriptor* pipeline_descriptor = [MTLRenderPipelineDescriptor new];
    pipeline_descriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
    pipeline_descriptor.vertexFunction = default_vshader_;
    pipeline_descriptor.fragmentFunction = default_fragshader_;

    render_pipeline_ = [device_ newRenderPipelineStateWithDescriptor:pipeline_descriptor error:&err];

    if ( render_pipeline_ == nil ) {
        SKY_ERROR("Graphics Device Interface", "Couldn't initialize main render pipeline state: NSError: %s",
                  [[err localizedDescription] UTF8String]);
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

bool MetalGDI::create_vertex_buffer(const uint32_t vbuf_id, const MemoryBlock& initial_data,
                                    const BufferUsage usage)
{
    vertex_buffers_.create(vbuf_id);
    vertex_buffers_.lookup(vbuf_id)->init(device_, initial_data.data, initial_data.size, usage);
    return true;
}

bool MetalGDI::set_vertex_buffer(const uint32_t vbuf_id)
{
    [render_encoder_ setVertexBuffer:vertex_buffers_.lookup(vbuf_id)->current()
                               offset:0
                              atIndex:0];
    [render_encoder_ drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
    return true;
}

bool MetalGDI::create_shader(const uint32_t shader_id, const char* name)
{
    NSString* nsname = [NSString stringWithUTF8String:name];
    id<MTLFunction> func = [library_ newFunctionWithName:nsname];

    if ( func == nil ) {
        SKY_ERROR("Shader", "Couldn't create shader with name %s", name);
        return false;
    }

    shaders_.create(shader_id, func);
    return true;
}

bool MetalGDI::set_shaders(const uint32_t vertex_id, const uint32_t fragment_id)
{
    if ( !shaders_.contains(vertex_id) || vertex_id == shaders_.invalid_id ) {
        SKY_ERROR("Shader", "%" PRIu32 " is an invalid vertex shader ID", vertex_id);
        return false;
    }
    if ( !shaders_.contains(fragment_id) || fragment_id == shaders_.invalid_id ) {
        SKY_ERROR("Shader", "%" PRIu32 " is an invalid vertex shader ID", fragment_id);
        return false;
    }

    return false;
}

void MetalGDI::present()
{
    if ( mtl_layer_ == nil ) {
        return;
    }

    command_buffer_ = [command_queue_ commandBufferWithUnretainedReferences];

@autoreleasepool {

    id<CAMetalDrawable> drawable = [mtl_layer_ nextDrawable];
    if ( drawable == nil ) {
        SKY_ERROR("Renderer", "Couldn't get next CAMetalDrawable");
        return;
    }

    MTLRenderPassDescriptor* rpd = [MTLRenderPassDescriptor renderPassDescriptor];

    if ( rpd == nil ) {
        SKY_ERROR("Renderer", "Couldn't create a RenderPassDescriptor");
        return;
    }

    rpd.colorAttachments[0].texture = drawable.texture;
    rpd.colorAttachments[0].loadAction = MTLLoadActionClear;
    rpd.colorAttachments[0].clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 0.0);
    rpd.colorAttachments[0].storeAction = MTLStoreActionStore;

    render_encoder_ = [command_buffer_ renderCommandEncoderWithDescriptor:rpd];

    [render_encoder_ setRenderPipelineState:render_pipeline_];

    process_commands();

    [render_encoder_ endEncoding];
    [command_buffer_ presentDrawable:drawable];
};

    [command_buffer_ commit];
}


}  // namespace sky

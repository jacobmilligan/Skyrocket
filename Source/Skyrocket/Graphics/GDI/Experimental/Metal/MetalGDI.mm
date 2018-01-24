//
//  MetalGDI.mm
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 6/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Core/Hash.hpp"
#include "Skyrocket/Graphics/GDI/Experimental/Metal/MetalGDI.h"
#include "Skyrocket/Graphics/GDI/GDI.hpp"
#include "Skyrocket/Graphics/Apple/MacViewport.h"
#include "Skyrocket/Graphics/Apple/MetalView.h"
#include "Skyrocket/Platform/Filesystem.hpp"

//TODO(Jacob): Textures

namespace sky {
namespace experimental {


MetalProgram::MetalProgram(const uint32_t program_id, id<MTLFunction> vs, id<MTLFunction> frag)
    : program_id_(program_id), vs_(vs), frag_(frag)
{}

MetalProgram::~MetalProgram()
{}

id<MTLRenderPipelineState> MetalProgram::get_render_pipeline_state(id<MTLDevice> device)
{
    auto flags = program_id_ + default_state_flags;
    auto hash = hash::murmur3_32(&flags, sizeof(uint32_t), 0);
    auto rps = render_pipeline_states_.find(hash);

    if ( rps != render_pipeline_states_.end() ) {
        return rps->second;
    }

    if ( vs_ == nil ) {
        SKY_ERROR("Render Pipeline",
                  "Cannot get new render pipeline state: Vertex function is nil");
        return nil;
    }

    if ( frag_ == nil ) {
        SKY_ERROR("Render Pipeline",
                  "Cannot get new render pipeline state: Fragment function is nil");
        return nil;
    }

    NSError* err = nil;

    MTLRenderPipelineDescriptor* pipeline_descriptor = [MTLRenderPipelineDescriptor new];
    pipeline_descriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
    pipeline_descriptor.vertexFunction = vs_;
    pipeline_descriptor.fragmentFunction = frag_;
    pipeline_descriptor.colorAttachments[0].blendingEnabled = YES;
    pipeline_descriptor.colorAttachments[0].rgbBlendOperation = MTLBlendOperationAdd;
    pipeline_descriptor.colorAttachments[0].alphaBlendOperation = MTLBlendOperationAdd;
    pipeline_descriptor.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
    pipeline_descriptor.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
    pipeline_descriptor.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    pipeline_descriptor.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusBlendAlpha;

    id<MTLRenderPipelineState> new_rps = [device newRenderPipelineStateWithDescriptor:pipeline_descriptor
    error:&err];



    if ( new_rps == nil ) {
        SKY_ERROR("Render Pipeline",
                  "Couldn't initialize main render pipeline state: NSError: %s",
                  [[err localizedDescription] UTF8String]);
        return nil;
    }

    render_pipeline_states_.insert({hash, new_rps});
    return new_rps;
}

///////////////////////////
///  GDI Implementation
///////////////////////////

#if SKY_COMPILER_MSVC != 1

constexpr MTLPixelFormat MetalGDI_EX::mtl_pixel_formats[];

#endif

MetalGDI_EX::MetalGDI_EX() = default;

MetalGDI_EX::~MetalGDI_EX()
{
    for ( int i = 0; i < max_frames_in_flight; ++i ) {
        dispatch_semaphore_signal(buf_sem_);
    }
}

std::unique_ptr<GDI_EX> GDI_EX::create() noexcept
{
    return std::make_unique<MetalGDI_EX>();
}

bool MetalGDI_EX::init(Viewport* viewport)
{
    //---------------------------------------
    //  Initialize device and command queue
    //---------------------------------------

    NSError * err = nil;
    device_ = MTLCreateSystemDefaultDevice();

    set_viewport(viewport);

    command_queue_ = [device_ newCommandQueue];
    buf_sem_ = dispatch_semaphore_create(max_frames_in_flight);

    //--------------------------------
    //  Load library and shader path
    //--------------------------------
//    Path lib_path(SKY_RESOURCE_DIRECTORY);
//    lib_path.append("Shaders/Metal/Lib.metallib");
//
//    NSString* nspath = [NSString stringWithUTF8String:lib_path.str()];
//    library_ = [device_ newLibraryWithFile:nspath error:&err];
//
//    if ( library_ == nil ) {
//        SKY_ERROR("Cubes Device Interface", "Couldn't load metal library with path %s: NSError: %s",
//                  lib_path.str(), [[err localizedDescription] UTF8String]);
//        return false;
//    }

    //--------------------------------
    //  Load default library
    //--------------------------------

    const char* default_src = R"(
#include <metal_stdlib>

using namespace metal;

struct Vertex {
    float4 position [[position]];
    float4 color;
    float2 tex_coords;
};

vertex Vertex basic_vertex(device Vertex* vertices [[buffer(0)]],
                           uint vid [[vertex_id]])
{
    return vertices[vid];
}


fragment float4 basic_fragment(Vertex in [[stage_in]])
{
    return in.color;
}
    )";

    NSString* nssrc = [NSString stringWithUTF8String:default_src];

    id<MTLLibrary> lib = [device_ newLibraryWithSource:nssrc
    options:nil
    error:&err];
    if ( lib == nil ) {
        SKY_ASSERT(default_library_ != nil,
                   "Default Metal Library loads correctly (see NSError: %s)",
                   [[err localizedDescription] UTF8String]);
        return false;
    }

    id<MTLFunction> vs = [lib newFunctionWithName:@"basic_vertex"];
    id<MTLFunction> frag = [lib newFunctionWithName:@"basic_fragment"];

    default_program_ = MetalProgram(0, vs, frag);

    //--------------------------------
    //  Load main render pipeline
    //--------------------------------

    MTLDepthStencilDescriptor* ds_descriptor = [MTLDepthStencilDescriptor new];
    ds_descriptor.depthCompareFunction = MTLCompareFunctionLess;
    ds_descriptor.depthWriteEnabled = YES;
    depth_stencil_state_ = [device_ newDepthStencilStateWithDescriptor:ds_descriptor];

    return true;
}



void MetalGDI_EX::commit(CommandBuffer* cmdbuf)
{
    dispatch_semaphore_wait(buf_sem_, DISPATCH_TIME_FOREVER);

    if ( mtl_layer_ == nil ) {
        SKY_ERROR("Drawing", "Could not commit - no Metal layer specified");
        return;
    }

    @autoreleasepool {
        id<MTLCommandBuffer> cmd_buffer = [command_queue_ commandBufferWithUnretainedReferences];

        MTLRenderPassDescriptor* rpd = [MTLRenderPassDescriptor renderPassDescriptor];

        if ( rpd == nil ) {
            SKY_ERROR("Renderer", "Couldn't create a RenderPassDescriptor");
            return;
        }

        id<CAMetalDrawable> drawable = [mtl_layer_ nextDrawable];
        if ( drawable == nil ) {
            SKY_ERROR("Renderer", "Couldn't get next CAMetalDrawable");
            return;
        }

        rpd.colorAttachments[0].texture = drawable.texture;
        rpd.colorAttachments[0].loadAction = MTLLoadActionClear;
        rpd.colorAttachments[0].clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 0.0);
        rpd.colorAttachments[0].storeAction = MTLStoreActionStore;

        render_encoder_ = [cmd_buffer renderCommandEncoderWithDescriptor:rpd];

        if ( render_pipeline_ == nil ) {
            render_pipeline_ = default_program_.get_render_pipeline_state(device_);
        }

        [render_encoder_ setRenderPipelineState:render_pipeline_];

        [render_encoder_ setDepthStencilState:depth_stencil_state_];
        [render_encoder_ setCullMode:MTLCullModeBack];

        execute_commands(cmdbuf);

        [render_encoder_ endEncoding];

        [cmd_buffer presentDrawable:drawable];
        [cmd_buffer addCompletedHandler:^(id<MTLCommandBuffer> commandBuffer){
            buffer_index_ = (buffer_index_ + 1) % max_frames_in_flight;
            dispatch_semaphore_signal(buf_sem_);
        }];

        [cmd_buffer commit];
    };
}

///////////////////////////
///  GDI Commands
///////////////////////////

void MetalGDI_EX::set_viewport(Viewport* viewport)
{
    MetalView * mtl_view = (MetalView * )
        viewport->get_native_viewport()->view;
    mtl_view.metalLayer.device = device_;
    mtl_layer_ = mtl_view.metalLayer;
}

bool MetalGDI_EX::create_vertex_buffer(const uint32_t vbuf_id, const MemoryBlock& initial_data,
                                       const BufferUsage usage)
{
    vertex_buffers_.create(vbuf_id);

    auto vbuf = vertex_buffers_.lookup(vbuf_id);

    if ( vbuf == nullptr ) {
        SKY_ERROR("Vertex Buffer", "Could not create a new vertex buffer with id %"
            PRIu32, vbuf_id);
        return false;
    }

    vbuf->init(device_, initial_data.data, initial_data.size, usage);

    return true;
}

bool MetalGDI_EX::set_vertex_buffer(const uint32_t vbuf_id)
{
    auto vbuf = vertex_buffers_.lookup(vbuf_id);

    if ( vbuf == nullptr ) {
        SKY_ERROR("Vertex Buffer", "Invalid vertex buffer specified with ID %"
            PRIu32, vbuf_id);
        return false;
    }

    [render_encoder_ setVertexBuffer:
    vbuf->raw_buffer()
    offset:0
    atIndex:0];
    return true;
}

bool MetalGDI_EX::create_index_buffer(const uint32_t ibuf_id, const MemoryBlock& initial_data)
{
    index_buffers_.create(ibuf_id);
    auto ibuf = index_buffers_.lookup(ibuf_id);

    if ( ibuf == nullptr ) {
        SKY_ERROR("Index Buffer", "Could not create a new vertex buffer with id %"
            PRIu32, ibuf_id);
        return false;
    }

    ibuf->init(device_, initial_data.data, initial_data.size, BufferUsage::none);

    return true;
}

bool MetalGDI_EX::set_index_buffer(const uint32_t vbuf_id)
{
    // No op - just need to specify buffer with Metal (done in process_commands())
    return GDI_EX::set_index_buffer(vbuf_id);
}

bool MetalGDI_EX::create_program(const uint32_t program_id, const Path& vs_path, const Path& frag_path)
{
    NSError* err = nil;
    id<MTLLibrary> lib = nil;

    auto make_function = [&](const Path& path) {
        id<MTLFunction> func = nil;
        auto src = fs::slurp_file(path);

        lib = [device_ newLibraryWithSource:[NSString stringWithUTF8String:src.c_str()]
        options:nil
        error:&err];
        if ( lib == nil ) {
            SKY_ERROR("Shader", "Couldn't load metal shader library (see NSError: %s)",
                      [[err localizedDescription] UTF8String]);
        } else {
            std::string stem = path.stem();
            NSString* func_name = [NSString stringWithUTF8String:stem.c_str()];
            func = [lib newFunctionWithName:func_name];
        }

        return func;
    };

    id<MTLFunction> vs = make_function(vs_path);
    id<MTLFunction> frag = make_function(frag_path);

    // Create program
    MetalProgram program(program_id, vs, frag);
    programs_.create(program_id, program);
    return true;
}


bool MetalGDI_EX::set_program(const uint32_t program_id)
{
    NSError* err = nil;
    auto* program = programs_.lookup(program_id);
    render_pipeline_ = program->get_render_pipeline_state(device_);

    if ( render_pipeline_ == nil ) {
        SKY_ERROR("GDI",
                  "Couldn't initialize main render pipeline state: NSError: %s",
                  [[err localizedDescription] UTF8String]);
        return false;
    }

    return true;
}

bool MetalGDI_EX::create_uniform(const uint32_t u_id, const uint32_t size)
{
    uniform_buffers_.create(u_id);

    auto ubuf = uniform_buffers_.lookup(u_id);

    if ( ubuf == nullptr ) {
        SKY_ERROR("Uniform", "Could not create a new uniform buffer with id %" PRIu32, u_id);
        return false;
    }

    ubuf->init(device_, nullptr, size, BufferUsage::none);

    return true;
}

void MetalGDI_EX::set_uniform(const uint32_t u_id, const uint32_t index)
{
    auto ubuf = uniform_buffers_.lookup(u_id);

    if ( ubuf == nullptr ) {
        SKY_ERROR("Uniform", "Invalid uniform specified with ID of %" PRIu32, u_id);
        return;
    }

    [render_encoder_ setVertexBuffer:
    ubuf->raw_buffer()
    offset:0
    atIndex:index];
}

void MetalGDI_EX::update_uniform(const uint32_t u_id, const MemoryBlock& data, const uint32_t offset)
{
    auto ubuf = uniform_buffers_.lookup(u_id);

    if ( ubuf == nullptr ) {
        SKY_ERROR("Uniform", "Invalid uniform specified with ID of %" PRIu32, u_id);
        return;
    }

    auto* dest = static_cast<uint8_t*>([ubuf->raw_buffer() contents]);
    memcpy(dest + offset, data.data, data.size);
}

void MetalGDI_EX::create_texture(const uint32_t t_id, const uint32_t width,
                                 const uint32_t height, const PixelFormat::Enum pixel_format,
                                 const bool mipmapped)
{
    auto mtl_format = mtl_pixel_formats[pixel_format];
    auto* descriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:mtl_format
    width:width
    height:height
    mipmapped:mipmapped];
    textures_.create(t_id, [device_ newTextureWithDescriptor:descriptor]);
}

void MetalGDI_EX::create_texture_region(const uint32_t tex_id, const UIntRect& region,
                                        const PixelFormat::Enum pixel_format, uint8_t* data)
{
    auto bytes_per_pixel = PixelFormat::bytes_per_pixel(pixel_format);
    auto tex = textures_.lookup(tex_id);
    auto bpr = bytes_per_pixel * region.width;
    MTLRegion mtl_region = MTLRegionMake2D(region.position.x, region.position.y, region.width, region.height);
    [*tex replaceRegion:mtl_region
    mipmapLevel:0
    withBytes:data
    bytesPerRow:bpr];
}

void MetalGDI_EX::set_texture(const uint32_t t_id, const uint32_t index)
{
    auto* tex = textures_.lookup(t_id);
    if ( t_id == textures_.invalid_id ) {
        SKY_ERROR("Texture", "Invalid texture ID specified");
        return;
    }

    [render_encoder_ setFragmentTexture:*tex atIndex:index];
}

void MetalGDI_EX::set_state(const uint32_t flags)
{
    if ( ( 0 | RenderPipelineState::culling_none
        | RenderPipelineState::culling_backface
        | RenderPipelineState::culling_frontface)
        & flags ) {

        if ( RenderPipelineState::culling_none & flags ) {
            [render_encoder_ setCullMode:MTLCullModeNone];
        }

        if ( RenderPipelineState::culling_backface & flags ) {
            [render_encoder_ setCullMode:MTLCullModeBack];
        }

        if ( RenderPipelineState::culling_frontface & flags ) {
            [render_encoder_ setCullMode:MTLCullModeFront];
        }

    }
}

bool MetalGDI_EX::draw()
{
    if ( state_.index_buffer > 0 ) {
        [render_encoder_ drawIndexedPrimitives:MTLPrimitiveTypeTriangle
        indexCount:state_.index_count
        indexType:MTLIndexTypeUInt32
        indexBuffer:
        index_buffers_.lookup(state_.index_buffer)->raw_buffer()
        indexBufferOffset:state_.index_offset];
    } else {
        [render_encoder_ drawPrimitives:MTLPrimitiveTypeTriangle
        vertexStart:state_.vertex_offset
        vertexCount:state_.vertex_count
        instanceCount:1];
    }

    return true;
}

bool MetalGDI_EX::draw_instanced(const uint32_t instance)
{
    if ( state_.index_buffer > 0 ) {
        [render_encoder_ drawIndexedPrimitives:MTLPrimitiveTypeTriangle
        indexCount:state_.index_count
        indexType:MTLIndexTypeUInt32
        indexBuffer:
        index_buffers_.lookup(state_.index_buffer)->raw_buffer()
        indexBufferOffset:state_.index_offset
        instanceCount:instance];
    } else {
        [render_encoder_ drawPrimitives:MTLPrimitiveTypeTriangle
        vertexStart:state_.vertex_offset
        vertexCount:state_.vertex_count
        instanceCount:instance];
    }

    return true;
}


}
}  // namespace sky

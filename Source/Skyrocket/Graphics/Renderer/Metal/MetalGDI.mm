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

#include "Skyrocket/Core/Math.hpp"
#include "Skyrocket/Graphics/Renderer/Metal/MetalGDI.h"
#include "Skyrocket/Graphics/Renderer/Metal/MetalView.h"
#include "Skyrocket/Graphics/Apple/MacViewport.h"

//TODO(Jacob): Textures

namespace sky {


std::unique_ptr<GDI> gdi_create_metal()
{
    return std::make_unique<MetalGDI>();
}


///////////////////////////
///  GDI Implementation
///////////////////////////

#if SKY_COMPILER_MSVC != 1

constexpr MTLPixelFormat MetalGDI::mtl_pixel_formats_[];
constexpr MTLSamplerMipFilter MetalGDI::mtl_mipmap_filter_[];
constexpr MTLSamplerMinMagFilter MetalGDI::mtl_minmag_filter_[];
constexpr MTLSamplerAddressMode MetalGDI::mtl_texwrap_mode_[];

#endif

MetalGDI::MetalGDI() = default;

MetalGDI::~MetalGDI()
{
    destroy();
}

bool MetalGDI::init(Viewport* viewport)
{
    Matrix::depth = ClipSpaceDepth::zero_to_one;

    @autoreleasepool {

        render_pipeline_ = nil;
        //---------------------------------------
        //  Initialize device and command queue
        //---------------------------------------

        NSError * err = nil;
        device_ = MTLCreateSystemDefaultDevice();

        current_view_ = viewport;

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

vertex Vertex default_vertex(device Vertex* vertices [[buffer(0)]],
                           uint vid [[vertex_id]])
{
    return vertices[vid];
}


fragment float4 default_fragment(Vertex in [[stage_in]])
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

        id<MTLFunction> vs = [lib newFunctionWithName:@"default_vertex"];
        id<MTLFunction> frag = [lib newFunctionWithName:@"default_fragment"];

        shadecc::ShaderSource src_obj = {};
        default_program_ = MetalProgram(0);
        default_program_.create_from_functions(device_, vs, frag, false, false);

        //--------------------------------
        //  Load main render pipeline
        //--------------------------------

        MTLDepthStencilDescriptor* ds_descriptor = [[MTLDepthStencilDescriptor alloc] init];
        ds_descriptor.depthCompareFunction = MTLCompareFunctionLess;
        ds_descriptor.depthWriteEnabled = YES;
        depth_stencil_state_ = [device_ newDepthStencilStateWithDescriptor:ds_descriptor];

        SKY_OBJC_RELEASE(lib);
    }

    return true;
}

bool MetalGDI::destroy()
{
    if (pool_) {
        [pool_ drain];
        pool_ = nil;
    }

    for (auto& p : programs_) {
        p.data.destroy();
    }

    for (auto& v : vertex_buffers_) {
        v.data.destroy();
    }

    for (auto& i : index_buffers_) {
        i.data.destroy();
    }

    for (auto& u : uniform_buffers_) {
        u.data.destroy();
    }

    for (auto& t : textures_) {
        SKY_OBJC_RELEASE(t.data);
    }

    SKY_OBJC_RELEASE(device_);

    return true;
}

bool MetalGDI::begin_frame(FrameInfo* frame_info)
{
    if (device_ == nil || buf_sem_ == nil || !current_view_->is_open()) {
        return false;
    }

    dispatch_semaphore_wait(buf_sem_, DISPATCH_TIME_FOREVER);

    if ( current_view_ == nullptr ) {
        SKY_ERROR("Drawing", "Could not commit - no Metal layer specified");
        return false;
    }

    auto mtlview = (MetalView*)current_view_->get_native_handle()->view;
    if (mtlview.metalLayer.device == nil) {
        ((MetalView*)current_view_->get_native_handle()->view).metalLayer.device = device_;
    }

    // Create an
    pool_ = [[NSAutoreleasePool alloc] init];

    mtlcmdbuf_ = [command_queue_ commandBufferWithUnretainedReferences];

    MTLRenderPassDescriptor* rpd = [MTLRenderPassDescriptor renderPassDescriptor];

    if (rpd == nil) {
        SKY_ERROR("Renderer", "Couldn't create a RenderPassDescriptor");
        return false;
    }

    mtldrawable_ = [mtlview.metalLayer nextDrawable];
    if (mtldrawable_ == nil) {
        SKY_ERROR("Renderer", "Couldn't get next CAMetalDrawable");
        return false;
    }

    rpd.colorAttachments[0].texture = mtldrawable_.texture;
    rpd.colorAttachments[0].loadAction = MTLLoadActionClear;
    rpd.colorAttachments[0].clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 0.0);
    rpd.colorAttachments[0].storeAction = MTLStoreActionStore;

    render_encoder_ = [mtlcmdbuf_ renderCommandEncoderWithDescriptor:rpd];

    if (render_pipeline_ == nil) {
        render_pipeline_ = default_program_.get_render_pipeline_state(device_);
    }

    [render_encoder_ setRenderPipelineState:render_pipeline_];
    [render_encoder_ setDepthStencilState:depth_stencil_state_];
    [render_encoder_ setCullMode:MTLCullModeBack];
    [render_encoder_ setFrontFacingWinding:MTLWindingCounterClockwise];

    return true;
}

bool MetalGDI::end_frame(FrameInfo* frame_info)
{
    [render_encoder_ endEncoding];
    [mtlcmdbuf_ presentDrawable:mtldrawable_];
    frame_info->gpu_start();

    [mtlcmdbuf_ addCompletedHandler:^(id<MTLCommandBuffer> commandBuffer){
        buffer_index_ = (buffer_index_ + 1) % max_frames_in_flight;

        dispatch_semaphore_signal(buf_sem_);

        frame_info->gpu_end();
    }];

    [mtlcmdbuf_ commit];

    frame_info->cpu_end();

    [pool_ drain];
    pool_ = nil;

    return true;
}

///////////////////////////
///  GDI Commands
///////////////////////////

void MetalGDI::set_viewport(Viewport* viewport)
{
    current_view_ = viewport;
    auto mtlview = (MetalView*)current_view_->get_native_handle()->view;
    mtlview.metalLayer.device = device_;

}

void MetalGDI::set_clear_color(const Color& color)
{
    [current_view_->get_native_handle()->view setBackingColor:((CGFloat) color.r) / 255.0
                                                            g:((CGFloat) color.g) / 255.0
                                                            b:((CGFloat) color.b) / 255.0
                                                            a:((CGFloat) color.a) / 255.0];
}

bool MetalGDI::create_vertex_buffer(const uint32_t vbuf_id, const MemoryBlock& initial_data,
                                    const BufferUsage usage)
{
    auto vbuf = vertex_buffers_.create(vbuf_id);

    if ( vbuf == nullptr ) {
        SKY_ERROR("Vertex Buffer", "Could not create a new vertex buffer with id %"
            PRIu32, vbuf_id);
        return false;
    }

    vbuf->init(device_, initial_data.data, initial_data.size, usage);

    return true;
}

bool MetalGDI::set_vertex_buffer(const uint32_t vbuf_id)
{
    auto vbuf = vertex_buffers_.get(vbuf_id);

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

bool MetalGDI::update_vertex_buffer(uint32_t vbuf_id, const MemoryBlock& data)
{
    auto buf = vertex_buffers_.get(vbuf_id);
    if (buf != nullptr) {
        buf->update(device_, data.data, data.size);
        return true;
    }

    return false;
}

bool MetalGDI::create_index_buffer(const uint32_t ibuf_id, const MemoryBlock& initial_data)
{
    auto ibuf = index_buffers_.create(ibuf_id);

    if ( ibuf == nullptr ) {
        SKY_ERROR("Index Buffer", "Could not create a new vertex buffer with id %"
            PRIu32, ibuf_id);
        return false;
    }

    ibuf->init(device_, initial_data.data, initial_data.size, BufferUsage::none);

    return true;
}

bool MetalGDI::set_index_buffer(const uint32_t ibuf_id)
{
    // No op - just need to specify buffer with Metal (done in process_commands())
    return GDI::set_index_buffer(ibuf_id);
}

bool MetalGDI::create_program(uint32_t program_id, const shadecc::ShaderSource& vs_src,
                              const shadecc::ShaderSource& fs_src)
{
    // Create program
    MetalProgram program(program_id);
    program.create_from_source(device_, vs_src, fs_src);
    programs_.create(program_id, program);

    return true;
}


bool MetalGDI::set_program(const uint32_t program_id)
{
    SKY_ASSERT_GUARD(ag, "Setting a program", std::to_string(program_id).c_str());

    NSError* err = nil;
    MetalProgram* program = nullptr;
    if (program_id == 0) {
        program = &default_program_;
    } else {
        program = programs_.get(program_id);
    }

    render_pipeline_ = program->get_render_pipeline_state(device_);
    [render_encoder_ setRenderPipelineState:render_pipeline_];

    if ( render_pipeline_ == nil ) {
        SKY_ERROR("GDI",
                  "Couldn't initialize main render pipeline state: NSError: %s",
                  [[err localizedDescription] UTF8String]);
        return false;
    }

    return true;
}

bool MetalGDI::create_uniform(uint32_t u_id, const char* name, uint32_t size, UniformType type)
{
    auto ubuf = uniform_buffers_.create(u_id);

    if ( ubuf == nullptr ) {
        SKY_ERROR("Uniform", "Could not create a new uniform buffer with id %" PRIu32, u_id);
        return false;
    }

    ubuf->init(device_, nullptr, size, BufferUsage::none);
    return true;
}

bool MetalGDI::set_uniform(uint32_t u_id, uint32_t index)
{
    auto ubuf = uniform_buffers_.get(u_id);

    if ( ubuf == nullptr ) {
        SKY_ERROR("Uniform", "Invalid uniform specified with ID of %" PRIu32, u_id);
        return false;
    }

    [render_encoder_ setVertexBuffer:
                         ubuf->raw_buffer()
                              offset:0
                             atIndex:index];
    return true;
}

bool MetalGDI::update_uniform(const uint32_t u_id, const MemoryBlock& data, const uint32_t offset)
{
    auto ubuf = uniform_buffers_.get(u_id);

    if ( ubuf == nullptr ) {
        SKY_ERROR("Uniform", "Invalid uniform specified with ID of %" PRIu32, u_id);
        return false;
    }

    auto* dest = static_cast<uint8_t*>([ubuf->raw_buffer() contents]);
    memcpy(dest + offset, data.data, data.size);

    return true;
}

id<MTLSamplerState> MetalGDI::get_sampler_state(const SamplerStateDescriptor ss_descriptor)
{
    auto hash = ss_descriptor.get_hash();
    auto sampler = sampler_states_.find(hash);
    if (sampler != sampler_states_.end()) {
        return sampler->second;
    }

    auto mtl_descriptor = [[MTLSamplerDescriptor new] autorelease];

    auto wrap_mode = mtl_texwrap_mode_[static_cast<uint32_t>(ss_descriptor.texture_wrap_mode)];
    auto min_filter = mtl_minmag_filter_[static_cast<uint32_t>(ss_descriptor.min_filter)];
    auto mag_filter = mtl_minmag_filter_[static_cast<uint32_t>(ss_descriptor.mag_filter)];
    auto mip_filter = mtl_mipmap_filter_[static_cast<uint32_t>(ss_descriptor.mip_map_filter)];

    mtl_descriptor.sAddressMode = wrap_mode;
    mtl_descriptor.tAddressMode = wrap_mode;
    mtl_descriptor.rAddressMode = wrap_mode;
    mtl_descriptor.minFilter = min_filter;
    mtl_descriptor.magFilter = mag_filter;
    mtl_descriptor.mipFilter = mip_filter;
//    mtl_descriptor.lodMinClamp;
//    mtl_descriptor.lodMaxClamp;
//    mtl_descriptor.normalizedCoordinates;
//    mtl_descriptor.maxAnisotropy;

    auto sampler_state = [device_ newSamplerStateWithDescriptor:mtl_descriptor];
    sampler_states_.insert(std::make_pair(hash, sampler_state));
    return sampler_state;
}

bool MetalGDI::create_texture(uint32_t t_id, uint32_t width,
                              uint32_t height, PixelFormat pixel_format,
                              bool mipmapped)
{
    auto mtl_format = mtl_pixel_formats_[flag_type(pixel_format)];
    auto* descriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:mtl_format
                                                                          width:width
                                                                         height:height
                                                                      mipmapped:mipmapped];
    textures_.create(t_id, [device_ newTextureWithDescriptor:descriptor]);
    return true;
}

bool MetalGDI::create_texture_region(uint32_t tex_id, const UIntRect& region,
                                     PixelFormat pixel_format, uint8_t* data)
{
    auto bpp = pf_bytes_per_pixel(pixel_format);
    auto tex = textures_.get(tex_id);
    auto bpr = bpp * region.width;
    MTLRegion mtl_region = MTLRegionMake2D(region.position.x, region.position.y,
                                           region.width, region.height);
    [*tex replaceRegion:mtl_region
            mipmapLevel:0
              withBytes:data
            bytesPerRow:bpr];
    return true;
}

bool MetalGDI::set_texture(const uint32_t t_id, const uint32_t index)
{
    auto* tex = textures_.get(t_id);
    if ( t_id == textures_.invalid_id ) {
        SKY_ERROR("Texture", "Invalid texture ID specified");
        return false;
    }

    // TODO(Jacob): Add sampler states to the frontend as a user-specified thing
    SamplerStateDescriptor descriptor{};
    descriptor.mip_map_filter = MipMapFilter::none;
    descriptor.min_filter = MinMagFilter::linear;
    descriptor.mag_filter = MinMagFilter::linear;
    descriptor.texture_wrap_mode = TextureWrapMode::repeat;

    auto sampler_state = get_sampler_state(descriptor);

    [render_encoder_ setFragmentTexture:*tex atIndex:index];
    [render_encoder_ setFragmentSamplerState:sampler_state atIndex:index];

    return true;
}

bool MetalGDI::set_state(const uint32_t flags)
{
    if ( ( 0 | flag_type(PipelineStateFlags::culling_none)
        | flag_type(PipelineStateFlags::culling_backface)
        | flag_type(PipelineStateFlags::culling_frontface) )
        & flags ) {

        if ( flag_type(PipelineStateFlags::culling_none) & flags ) {
            [render_encoder_ setCullMode:MTLCullModeNone];
        }

        if ( flag_type(PipelineStateFlags::culling_backface) & flags ) {
            [render_encoder_ setCullMode:MTLCullModeBack];
        }

        if ( flag_type(PipelineStateFlags::culling_frontface) & flags ) {
            [render_encoder_ setCullMode:MTLCullModeFront];
        }

    }

    return true;
}

bool MetalGDI::draw()
{
    if ( state_.index_buffer > 0 ) {
        [render_encoder_ drawIndexedPrimitives:MTLPrimitiveTypeTriangle
                                    indexCount:state_.index_count
                                     indexType:MTLIndexTypeUInt32
                                   indexBuffer:index_buffers_.get(state_.index_buffer)->raw_buffer()
                             indexBufferOffset:state_.index_offset];
    } else {
        [render_encoder_ drawPrimitives:MTLPrimitiveTypeTriangle
                            vertexStart:state_.vertex_offset
                            vertexCount:state_.vertex_count
                          instanceCount:1];
    }

    return true;
}

bool MetalGDI::draw_instanced(const uint32_t instance)
{
    if ( state_.index_buffer > 0 ) {
        [render_encoder_ drawIndexedPrimitives:MTLPrimitiveTypeTriangle
                                    indexCount:state_.index_count
                                     indexType:MTLIndexTypeUInt32
                                   indexBuffer:index_buffers_.get(state_.index_buffer)->raw_buffer()
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

bool MetalGDI::create_instance_buffer(uint32_t inst_id, uint32_t stride, uint32_t size)
{
    auto buf = instance_buffers_.create(inst_id);

    if ( buf == nullptr ) {
        SKY_ERROR("Instance", "Could not create a new instance buffer with id %" PRIu32, inst_id);
        return false;
    }

    buf->stride = stride;
    buf->buffer.init(device_, nullptr, stride * size, BufferUsage::dynamic);

    return true;
}

bool MetalGDI::update_instance_buffer(uint32_t inst_id, uint8_t* data, uint32_t index)
{
    auto buf = instance_buffers_.get(inst_id);

    if ( buf == nullptr ) {
        SKY_ERROR("Instance", "Invalid instance buffer specified with ID of %" PRIu32, inst_id);
        return false;
    }

    buf->buffer.update(device_, data, buf->stride, index * buf->stride);

    return true;
}

bool MetalGDI::set_instance_buffer(uint32_t inst_id, uint32_t index)
{
    auto buf = instance_buffers_.get(inst_id);

    if ( buf == nullptr ) {
        SKY_ERROR("Instance", "Invalid instance buffer specified with ID of %" PRIu32, inst_id);
        return false;
    }

    [render_encoder_ setVertexBuffer:buf->buffer.raw_buffer()
                              offset:sizeof(float) * 10
                             atIndex:0];
    return true;
}


}  // namespace sky

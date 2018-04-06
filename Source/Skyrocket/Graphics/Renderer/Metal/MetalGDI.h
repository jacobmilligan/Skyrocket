//
//  MetalGDI.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 22/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

//TODO(Jacob): make some kind of metal buffer pool class for handling semaphores and multi-buffering

#pragma once

#if SKY_GRAPHICS_API_METAL == 1

#include "Skyrocket/Core/Containers/HandleTable.hpp"
#include "Skyrocket/Graphics/Renderer/GDI.hpp"
#include "Skyrocket/Graphics/Renderer/Metal/MetalResource.h"

#import <AppKit/AppKit.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

@class MetalView;

namespace sky {


class MetalGDI : public GDI {
public:
    MetalGDI();

    ~MetalGDI() override;

    bool init(Viewport* viewport) override;

    bool destroy() override;

    bool begin_frame(FrameInfo* frame_info) override;
    bool end_frame(FrameInfo* frame_info) override;

    void set_viewport(Viewport* viewport) override;

    void set_clear_color(const Color& color) override;
protected:
    bool create_vertex_buffer(uint32_t vbuf_id, const MemoryBlock& initial_data,
                              BufferUsage usage) override;

    bool set_vertex_buffer(uint32_t vbuf_id) override;

    bool update_vertex_buffer(uint32_t vbuf_id, const MemoryBlock& data) override;

    bool create_index_buffer(uint32_t ibuf_id, const MemoryBlock& initial_data) override;

    bool set_index_buffer(uint32_t ibuf_id) override;

    bool create_program(uint32_t shader_id, const shadecc::ShaderSource& vs_src, const shadecc::ShaderSource& fs_src) override;

    bool set_program(uint32_t program_id) override;

    bool create_uniform(uint32_t u_id, const char* name, uint32_t size, UniformType type) override;

    bool set_uniform(uint32_t u_id, uint32_t index) override;

    bool update_uniform(uint32_t u_id, const MemoryBlock& data, uint32_t offset) override;

    bool create_texture(uint32_t t_id, uint32_t width,
                        uint32_t height, PixelFormat::Enum pixel_format,
                        bool mipmapped) override;

    bool create_texture_region(uint32_t tex_id, const UIntRect& region,
                               PixelFormat::Enum pixel_format, uint8_t* data) override;

    bool set_texture(uint32_t t_id, uint32_t index) override;

    bool draw() override;

    bool draw_instanced(uint32_t instance) override;

    bool set_state(uint32_t flags) override;
    bool create_instance_buffer(uint32_t inst_id, uint32_t stride, uint32_t size) override;
    bool update_instance_buffer(uint32_t inst_id, uint8_t* data, uint32_t index) override;
    bool set_instance_buffer(uint32_t inst_id, uint32_t index) override;
private:
    static constexpr uint8_t lib_max_ = 8;

    static constexpr MTLPixelFormat mtl_pixel_formats_[] = {
        MTLPixelFormatR8Unorm, // r8
        MTLPixelFormatR16Unorm, // r16
        MTLPixelFormatR32Float, // r32
        MTLPixelFormatRG8Unorm, // rg8
        MTLPixelFormatRG16Unorm, // rg16
        MTLPixelFormatRG32Float, // rg32
        MTLPixelFormatRGBA8Unorm, // rgb8
        MTLPixelFormatBGRA8Unorm, // bgra8
        MTLPixelFormatRGBA8Unorm, // rgba8
        MTLPixelFormatRGBA16Unorm, // rgba16
        MTLPixelFormatRGBA32Float, // rgba32
        MTLPixelFormatDepth32Float, // depth
        MTLPixelFormatStencil8, // stencil
        MTLPixelFormatInvalid // unknown
    };

    static constexpr size_t mtl_pixelfmt_tablesize = sizeof(mtl_pixel_formats_) / sizeof(MTLPixelFormat);

    static_assert(mtl_pixelfmt_tablesize == static_cast<size_t>(PixelFormat::Enum::unknown) + 1,
                  "Skyrocket: Metal error: the translation table for PixelFormat "
                      "is missing entries. Please update to sync with the PixelFormat enum.");

    NSAutoreleasePool* pool_;
    id<MTLDevice> device_;
    id<MTLCommandQueue> command_queue_;
    id<MTLRenderPipelineState> render_pipeline_;

    id <CAMetalDrawable> mtldrawable_;
    id<MTLCommandBuffer> mtlcmdbuf_;
    id<MTLRenderCommandEncoder> render_encoder_;

    id<MTLLibrary> default_library_;

    MetalProgram default_program_;

    id<MTLDepthStencilState> depth_stencil_state_;

    dispatch_semaphore_t buf_sem_;

    Viewport* current_view_;

    HandleTable<MetalBuffer<max_frames_in_flight>, vertex_buffer_max> vertex_buffers_;
    HandleTable<MetalBuffer<max_frames_in_flight>, index_buffer_max> index_buffers_;
    HandleTable<MetalBuffer<max_frames_in_flight>, uniform_buffer_max> uniform_buffers_;
    HandleTable<MetalInstanceBuffer<max_frames_in_flight>, instance_buffer_max> instance_buffers_;
    HandleTable<MetalProgram, shader_max> programs_;
    HandleTable<id<MTLTexture>, texture_max> textures_;

    uint32_t buffer_index_{0};
};


}  // namespace sky

#endif // SKY_GRAPHICS_API_METAL

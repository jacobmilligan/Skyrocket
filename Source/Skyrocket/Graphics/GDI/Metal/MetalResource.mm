//
//  MetalResource.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 30/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Core/Diagnostics/Error.hpp"
#include "Skyrocket/Core/Hash.hpp"
#include "Skyrocket/Graphics/GDI/Metal/MetalResource.h"

namespace sky {


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


}
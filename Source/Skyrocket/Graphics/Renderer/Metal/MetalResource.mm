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
#include "Skyrocket/Graphics/Renderer/Metal/MetalResource.h"

namespace sky {


MetalProgram::MetalProgram(const uint32_t program_id, id<MTLFunction> vs, id<MTLFunction> frag)
    : program_id_(program_id), vs_(vs), frag_(frag)
{}

void MetalProgram::destroy()
{
    [vs_ release];
    [frag_ release];

    for (auto& s : render_pipeline_states_) {
        SKY_OBJC_RELEASE(s.second);
    }
}

id<MTLRenderPipelineState> MetalProgram::get_render_pipeline_state(id<MTLDevice> device)
{
    auto flags = program_id_ + default_state_flags;
    auto hash = hash::murmur3_32(&flags, sizeof(uint32_t), 0);
    auto rps = render_pipeline_states_.find(hash);

    if ( rps != render_pipeline_states_.end() ) {
        return rps->second;
    }

    SKY_ASSERT(vs_ != nil && frag_ != nil, "VS and FS functions are not nil")

    NSError* err = nil;

    MTLRenderPipelineDescriptor* pipeline_descriptor = [[MTLRenderPipelineDescriptor new] autorelease];
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

    SKY_ASSERT(new_rps != nil,
               "New render pipeline state failed to initialize with error: %s",
               [[err localizedDescription] UTF8String]);

    render_pipeline_states_.insert({hash, new_rps});
    return new_rps;
}


}
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


MetalProgram::MetalProgram(const uint32_t program_id)
    : program_id_(program_id), vs_is_instanced_(false), fs_is_instanced_(false)
{}

void MetalProgram::create_from_functions(id<MTLDevice> device, id<MTLFunction> vs,
                                         id<MTLFunction> fs, bool vs_is_instanced,
                                         bool fs_is_instanced)
{
    vs_ = vs;
    fs_ = fs;
    vs_is_instanced_ = vs_is_instanced;
    fs_is_instanced_ = fs_is_instanced;

    SKY_ASSERT(vs_ != nil && fs_ != nil, "VS and FS libraries compiled successfully");
}

void MetalProgram::create_from_source(id<MTLDevice> device, const shadecc::ShaderSource& vs,
                                      const shadecc::ShaderSource& fs)
{
    vs_ = make_mtl_function(device, vs);
    fs_ = make_mtl_function(device, fs);
    vs_is_instanced_ = vs.is_instanced;
    fs_is_instanced_ = fs.is_instanced;

    SKY_ASSERT(vs_ != nil && fs_ != nil, "VS and FS libraries compiled successfully");
}

void MetalProgram::destroy()
{
    SKY_OBJC_RELEASE(vs_);
    SKY_OBJC_RELEASE(fs_);

    for (auto& s : render_pipeline_states_) {
        SKY_OBJC_RELEASE(s.second);
    }
}

MTLVertexFormat MetalProgram::get_vertex_format(MTLVertexAttribute* attr)
{
    static constexpr MTLVertexFormat formats[] = {
        MTLVertexFormatInvalid, // MTLDataTypeNone
        MTLVertexFormatInvalid, // MTLDataTypeStruct
        MTLVertexFormatInvalid, // MTLDataTypeArray

        MTLVertexFormatFloat,   // MTLDataTypeFloat
        MTLVertexFormatFloat2,  // MTLDataTypeFloat2
        MTLVertexFormatFloat3,  // MTLDataTypeFloat3
        MTLVertexFormatFloat4,  // MTLDataTypeFloat4

        MTLVertexFormatInvalid, // MTLDataTypeFloat2x2
        MTLVertexFormatInvalid, // MTLDataTypeFloat2x3
        MTLVertexFormatInvalid, // MTLDataTypeFloat2x4

        MTLVertexFormatInvalid, // MTLDataTypeFloat3x2
        MTLVertexFormatInvalid, // MTLDataTypeFloat3x3
        MTLVertexFormatInvalid, // MTLDataTypeFloat3x4

        MTLVertexFormatInvalid, // MTLDataTypeFloat4x2
        MTLVertexFormatInvalid, // MTLDataTypeFloat4x3
        MTLVertexFormatInvalid, // MTLDataTypeFloat4x4

        MTLVertexFormatHalf2,   // MTLDataTypeHalf
        MTLVertexFormatHalf2,   // MTLDataTypeHalf2
        MTLVertexFormatHalf3,   // MTLDataTypeHalf3
        MTLVertexFormatHalf4,   // MTLDataTypeHalf4

        MTLVertexFormatInvalid, // MTLDataTypeHalf2x2
        MTLVertexFormatInvalid, // MTLDataTypeHalf2x3
        MTLVertexFormatInvalid, // MTLDataTypeHalf2x4

        MTLVertexFormatInvalid, // MTLDataTypeHalf3x2
        MTLVertexFormatInvalid, // MTLDataTypeHalf3x3
        MTLVertexFormatInvalid, // MTLDataTypeHalf3x4

        MTLVertexFormatInvalid, // MTLDataTypeHalf4x2
        MTLVertexFormatInvalid, // MTLDataTypeHalf4x3
        MTLVertexFormatInvalid, // MTLDataTypeHalf4x4

        MTLVertexFormatInt,     // MTLDataTypeInt
        MTLVertexFormatInt2,    // MTLDataTypeInt2
        MTLVertexFormatInt3,    // MTLDataTypeInt3
        MTLVertexFormatInt4,    // MTLDataTypeInt4

        MTLVertexFormatUInt,    // MTLDataTypeUInt
        MTLVertexFormatUInt2,   // MTLDataTypeUInt2
        MTLVertexFormatUInt3,   // MTLDataTypeUInt3
        MTLVertexFormatUInt4,   // MTLDataTypeUInt4

        MTLVertexFormatShort2,  // MTLDataTypeShort
        MTLVertexFormatShort2,  // MTLDataTypeShort2
        MTLVertexFormatShort3,  // MTLDataTypeShort3
        MTLVertexFormatShort4,  // MTLDataTypeShort4

        MTLVertexFormatUShort2, // MTLDataTypeUShort
        MTLVertexFormatUShort2, // MTLDataTypeUShort2
        MTLVertexFormatUShort3, // MTLDataTypeUShort3
        MTLVertexFormatUShort4, // MTLDataTypeUShort4

        MTLVertexFormatChar2,   // MTLDataTypeChar
        MTLVertexFormatChar2,   // MTLDataTypeChar2
        MTLVertexFormatChar3,   // MTLDataTypeChar3
        MTLVertexFormatChar4,   // MTLDataTypeChar4

        MTLVertexFormatUChar2,  // MTLDataTypeUChar
        MTLVertexFormatUChar2,  // MTLDataTypeUChar2
        MTLVertexFormatUChar3,  // MTLDataTypeUChar3
        MTLVertexFormatUChar4,  // MTLDataTypeUChar4

        MTLVertexFormatInvalid, // MTLDataTypeBool
        MTLVertexFormatInvalid, // MTLDataTypeBool2
        MTLVertexFormatInvalid, // MTLDataTypeBool3
        MTLVertexFormatInvalid, // MTLDataTypeBool4
    };

    static constexpr size_t format_tablesize = sizeof(formats) / sizeof(MTLVertexFormat);

    static_assert(format_tablesize == MTLDataTypeBool4 + 1,
                  "Skyrocket: Metal error: the translation table for MTLVertexFormat "
                  "is missing entries. Please update to sync with the MTLVertexFormat enum.");

    return formats[attr.attributeType];
}

id<MTLRenderPipelineState> MetalProgram::get_render_pipeline_state(id<MTLDevice> device)
{
    auto flags = program_id_ + default_state_flags;
    auto hash = hash::murmur3_32(&flags, sizeof(uint32_t), 0);
    auto rps = render_pipeline_states_.find(hash);

    if ( rps != render_pipeline_states_.end() ) {
        return rps->second;
    }

    SKY_ASSERT(vs_ != nil && fs_ != nil, "VS and FS functions are not nil")

    NSError* err = nil;
    MTLRenderPipelineDescriptor* pipeline_descriptor = [[MTLRenderPipelineDescriptor new] autorelease];

    // Add vertex attributes for each attribute in the shader
    if (vs_.vertexAttributes.count > 0) {
        auto vertexDescriptor = [MTLVertexDescriptor vertexDescriptor];
        for (uint32_t i = 0; i < vs_.vertexAttributes.count; ++i) {
            if (!vs_.vertexAttributes[i].active) {
                continue;
            }

            vertexDescriptor.attributes[i].format = get_vertex_format(vs_.vertexAttributes[i]);
            vertexDescriptor.attributes[i].bufferIndex = 0;
            vertexDescriptor.attributes[i].offset = sizeof(float) * 4 * i;
        }
        vertexDescriptor.layouts[0].stride = sizeof(float) * 4 * vs_.vertexAttributes.count;
        vertexDescriptor.layouts[0].stepFunction = vs_is_instanced_ ? MTLVertexStepFunctionPerInstance
                                                                    : MTLVertexStepFunctionPerVertex;
        vertexDescriptor.layouts[0].stepRate = 1;

        pipeline_descriptor.vertexDescriptor = vertexDescriptor;
    }

    pipeline_descriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
    pipeline_descriptor.vertexFunction = vs_;
    pipeline_descriptor.fragmentFunction = fs_;
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


id<MTLFunction> make_mtl_function(id<MTLDevice> device, const shadecc::ShaderSource& src)
{
    NSError* err = nil;
    auto lib = [device
        newLibraryWithSource:[NSString stringWithUTF8String:src.msl_src]
                     options:nil
                       error:&err];
    if ( lib == nil ) {
        SKY_ERROR("Shader", "Couldn't load metal shader library (see NSError: %s)",
                  [[err localizedDescription] UTF8String]);
        return nil;
    }

    auto name = "main0";
    NSString* func_name = [NSString stringWithUTF8String:name];
    auto func = [lib newFunctionWithName:func_name];

    if (func == nil) {
        SKY_ERROR("Metal", "Couldn't create function from source with name `%s`. "
                           "The file only contains the following functions: %s",
                  name, [[[lib functionNames] description] UTF8String]);
    }

    SKY_OBJC_RELEASE(lib);

    return func;
}


}
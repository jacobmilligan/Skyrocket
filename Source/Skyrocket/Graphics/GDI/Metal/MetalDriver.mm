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

#include "Skyrocket/Graphics/GDI/GDI.hpp"
#include "Skyrocket/Graphics/Internal/Apple/MetalView.h"

#include <memory>

#import <AppKit/AppKit.h>
#import <Metal/Metal.h>

namespace sky {
    
    
struct GDI::Context {
    id<MTLDevice> device;
    id<MTLCommandQueue> command_queue;

    void create()
    {
        device = MTLCreateSystemDefaultDevice();
        command_queue = [device newCommandQueue];
    }
};

GDI::GDI()
    : context_(nullptr)
{}

GDI::~GDI()
{}
    
void GDI::initialize()
{
    context_ = std::make_unique<Context>();
    context_->create();
}
    
    
}

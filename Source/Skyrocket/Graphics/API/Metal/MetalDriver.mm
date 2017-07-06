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

#include "Skyrocket/Graphics/Viewport.hpp"
#include "Skyrocket/Graphics/API/Metal/MetalDriver.h"
#include "Skyrocket/Platform/macOS/macOS.h"

#include <memory>

#import <AppKit/AppKit.h>
#import <Metal/Metal.h>

namespace sky {
    
std::unique_ptr<GraphicsDriver> GraphicsDriver::create(Viewport& viewport)
{
    auto driver = std::make_unique<MetalDriver>(viewport, (MetalView*)viewport.handle_);
    return std::move(driver);
}
    
MetalDriver::MetalDriver(Viewport& viewport, MetalView* view_handle)
    : GraphicsDriver(viewport)
{
    device_ = MTLCreateSystemDefaultDevice();
    view_ = view_handle;
    [view_ setDevice:device_];
    command_queue_ = [device_ newCommandQueue];
}
    
    
}

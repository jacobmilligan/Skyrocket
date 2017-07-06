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
#include "Skyrocket/Graphics/Interface/Apple/Metal/MetalView.h"
#include "Skyrocket/Graphics/Interface/Apple/Metal/MetalDriver.h"

#include <memory>

#import <AppKit/AppKit.h>
#import <Metal/Metal.h>

namespace sky {

std::unique_ptr<GraphicsDriver> GraphicsDriver::create()
{
    return std::make_unique<MetalDriver>();
}
    
MetalDriver::MetalDriver()
{
    device_ = MTLCreateSystemDefaultDevice();
    command_queue_ = [device_ newCommandQueue];
}
    
    
}

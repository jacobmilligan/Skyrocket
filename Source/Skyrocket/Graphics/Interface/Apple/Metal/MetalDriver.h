//
//  MetalDriver.h
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 6/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Platform/Config.hpp"

#if SKY_GRAPHICS_API_METAL

#include "Skyrocket/Graphics/Core/GraphicsDriver.hpp"

@protocol MTLDevice;
@protocol MTLCommandQueue;

namespace sky {

class MetalDriver : public GraphicsDriver {
public:
    MetalDriver();
private:
    id <MTLDevice> device_;
    id<MTLCommandQueue> command_queue_;
};


}

#endif // SKY_GRAPHICS_API_METAL
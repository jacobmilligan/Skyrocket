//
//  MetalView.h
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 6/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#if SKY_GRAPHICS_API_METAL

#include "Skyrocket/Graphics/Apple/CocoaView.h"

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

@interface MetalView : CocoaView

@property (readonly, nullable)CAMetalLayer* metalLayer;

@end

using view_type_t = MetalView;

#endif // Metal API

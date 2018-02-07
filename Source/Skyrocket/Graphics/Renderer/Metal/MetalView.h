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

#include "Skyrocket/Graphics/Apple/CocoaView.h"

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

@interface MetalView : CocoaView

@property (readonly, nullable)CAMetalLayer* metalLayer;

-(nullable id<CAMetalDrawable>)nextDrawable;

@end

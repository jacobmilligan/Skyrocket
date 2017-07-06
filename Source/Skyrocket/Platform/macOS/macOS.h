//
//  macOSPlatform.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 4/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Platform/Config.hpp"

#if SKY_OS_MACOS

#include <cstdint>

#import <AppKit/AppKit.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

namespace sky {
    class Application;
}

@interface SkyrocketApplication : NSApplication
@end

@interface SkyrocketApplicationDelegate : NSObject<NSApplicationDelegate>
@end

@interface CocoaView : NSView

@property (assign, nullable)sky::Application* app;

-(void)setBackingColor:(CGFloat)r
                     g:(CGFloat)g
                     b:(CGFloat)b
                     a:(CGFloat)a;

@end

@interface MetalView : CocoaView

@property (readonly, nullable)CAMetalLayer* metalLayer;

-(void)refresh;
-(void)setDevice:(nonnull id<MTLDevice>)device;

@end

#endif

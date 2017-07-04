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

@interface MetalView : NSView
    @property (assign, nullable)sky::Application* app;
    @property (assign, nonatomic, nullable)id <MTLDevice> device;
    @property (readonly, nullable)CAMetalLayer* metalLayer;
    @property (nonatomic, strong, nullable)id<MTLCommandQueue> commandQueue;

    -(void)refresh;
@end


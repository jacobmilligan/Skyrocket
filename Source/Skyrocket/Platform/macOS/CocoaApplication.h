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

namespace sky {
    class Application;
}

@interface SkyrocketApplication : NSApplication
@end

@interface SkyrocketApplicationDelegate : NSObject<NSApplicationDelegate>
@end

@interface MetalView : NSView
    @property (assign)sky::Application* app;
@end


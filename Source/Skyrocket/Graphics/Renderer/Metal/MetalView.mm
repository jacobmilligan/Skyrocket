//
//  MetalView.mm
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 4/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Platform/macOS/CocoaWindow.h"
#include "MetalView.h"

#import <QuartzCore/QuartzCore.h>

@implementation MetalView

-(id)initWithFrame:(NSRect)frameRect {
    if ( (self = [super initWithFrame:frameRect]) ) {
        [self setWantsLayer:YES];
        CGFloat bgColor[] = {0.0, 0.0, 0.0, 1.0};
        _metalLayer =[CAMetalLayer layer];
        
        _metalLayer.masksToBounds = YES;
        _metalLayer.presentsWithTransaction = NO;
        _metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        _metalLayer.framebufferOnly = YES;
        _metalLayer.frame = frameRect;
        _metalLayer.opaque = NO;
        _metalLayer.backgroundColor = CGColorCreate(CGColorSpaceCreateDeviceRGB(), bgColor);
        
        [_metalLayer removeAllAnimations];
        
        [self setLayer:_metalLayer];
    }
    return self;
}

-(id<CAMetalDrawable>)nextDrawable {
    return [_metalLayer nextDrawable];
}

-(void)setBackingColor:(CGFloat)r
                     g:(CGFloat)g
                     b:(CGFloat)b
                     a:(CGFloat)a
{
    CGFloat bgColor[] = {r, g, b, a};

    [CATransaction begin];
    _metalLayer.backgroundColor = CGColorCreate(CGColorSpaceCreateDeviceRGB(), bgColor);
    [CATransaction commit];
    [CATransaction flush];
}

@end

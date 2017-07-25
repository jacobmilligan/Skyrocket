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

#include "Skyrocket/Graphics/Viewport.hpp"
#include "Skyrocket/Platform/macOS/CocoaWindow.h"
#include "Skyrocket/Graphics/Internal/Apple/MetalView.h"

#import <AppKit/AppKit.h>

@implementation MetalView

-(id)initWithFrame:(NSRect)frameRect {
    if ( (self = [super initWithFrame:frameRect]) ) {
        [self setWantsLayer:YES];

        CGFloat bgColor[] = {0.0, 0.0, 0.0, 1.0};
        _metalLayer = [CAMetalLayer layer];

        _metalLayer.masksToBounds = YES;
        _metalLayer.presentsWithTransaction = NO;
        _metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        _metalLayer.framebufferOnly = YES;
        _metalLayer.frame = frameRect;
        _metalLayer.backgroundColor = CGColorCreate(CGColorSpaceCreateDeviceRGB(), bgColor);
        _metalLayer.opaque = NO;
        [_metalLayer removeAllAnimations];

        [self setLayer:_metalLayer];
    }

    return self;
}

-(void)setBackingColor:(CGFloat)r g:(CGFloat)g b:(CGFloat)b a:(CGFloat)a {
    CGFloat bgColor[] = {r, g, b, a};
    _metalLayer.backgroundColor = CGColorCreate(CGColorSpaceCreateDeviceRGB(), bgColor);
}

-(BOOL)acceptsFirstResponder {
    return YES;
}

-(BOOL)acceptsFirstMouse:(NSEvent *)event {
    return YES;
}

@end

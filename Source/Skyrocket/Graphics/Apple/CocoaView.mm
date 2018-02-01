//
//  CocoaView.mm
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 1/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Graphics/Apple/CocoaView.h"
#include "Skyrocket/Graphics/GraphicsDriver.hpp"

#import <CoreVideo/CoreVideo.h>

@implementation CocoaView {
    sky::GraphicsDriver* _graphicsDriver;
    sky::render_proc_t _frameCallback;
}

static CVReturn displayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now,
                                    const CVTimeStamp* outputTime, CVOptionFlags flagsIn,
                                    CVOptionFlags* flagsOut, void* displayLinkContext)
{

    CVReturn result = [(CocoaView*)displayLinkContext getFrameForTime:outputTime];
    return result;
}

-(void)setVsyncEnabled:(BOOL)enabled
        graphicsDriver:(sky::GraphicsDriver*)graphicsDriver
         frameCallback:(sky::render_proc_t)frameCallback
{
    if (enabled) {
        _graphicsDriver = graphicsDriver;
        _frameCallback = frameCallback;
        CVDisplayLinkCreateWithActiveCGDisplays(&_displayLink);
        CVDisplayLinkSetOutputCallback(_displayLink, &displayLinkCallback, self);
        CVDisplayLinkStart(_displayLink);
        return;
    }

    // Disabling vsync
    if (_displayLink != nil) {
        CVDisplayLinkRelease(_displayLink);
        _displayLink = nil;
    }
}

-(void)setBackingColor:(CGFloat)r g:(CGFloat) g b:(CGFloat)b a:(CGFloat)a {
// Empty - handled by subclasses
}

-(CVReturn)getFrameForTime:(const CVTimeStamp*)outputTime {
    (_graphicsDriver->*_frameCallback)();

    return kCVReturnSuccess;
}



@end
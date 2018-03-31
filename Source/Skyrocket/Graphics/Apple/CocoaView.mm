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
#include "Skyrocket/Graphics/Renderer/Renderer.hpp"

@implementation CocoaView {
    sky::Renderer* _renderer;
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
              renderer:(sky::Renderer*)renderer
         frameCallback:(sky::render_proc_t)frameCallback
{
    if (enabled) {
        _renderer = renderer;
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
    // otherwise no-op for metal
}

-(CVReturn)getFrameForTime:(const CVTimeStamp*)outputTime {
    (_renderer->*_frameCallback)();

    return kCVReturnSuccess;
}

-(BOOL)acceptsFirstResponder {
    return YES;
}

-(BOOL)acceptsFirstMouse:(NSEvent * )event {
    return YES;
}



@end
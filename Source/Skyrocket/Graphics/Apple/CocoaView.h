//
//  CocoaView.h
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 1/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#import <AppKit/AppKit.h>

namespace sky {

class Renderer;

using render_proc_t = void (Renderer::*)();

}

@interface CocoaView : NSView
{
    CVDisplayLinkRef _displayLink;
}

-(void)setVsyncEnabled:(BOOL)enabled
        graphicsDriver:(sky::Renderer*)graphicsDriver
         frameCallback:(sky::render_proc_t)frameCallback;

-(void)setBackingColor:(CGFloat)r
                     g:(CGFloat)g
                     b:(CGFloat)b
                     a:(CGFloat)a;

@end
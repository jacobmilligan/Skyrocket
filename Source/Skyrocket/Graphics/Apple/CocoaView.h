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

class GraphicsDriver;

using render_proc_t = void (GraphicsDriver::*)();

}

@interface CocoaView : NSView
{
    CVDisplayLinkRef _displayLink;
}

-(void)setVsyncEnabled:(BOOL)enabled
        graphicsDriver:(sky::GraphicsDriver*)graphicsDriver
         frameCallback:(sky::render_proc_t)frameCallback;

-(void)setBackingColor:(CGFloat)r
                     g:(CGFloat)g
                     b:(CGFloat)b
                     a:(CGFloat)a;

@end
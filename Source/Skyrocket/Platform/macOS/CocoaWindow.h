//
//  CocoaView.h
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 6/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#import <AppKit/AppKit.h>

namespace sky { class PlatformEvents; }


@interface CocoaWindow : NSWindow

@property (nonnull)sky::PlatformEvents* events;

-(nonnull instancetype)initWithEventsAndContent:(nonnull sky::PlatformEvents*)events
                                    contentRect:(NSRect)contentRect
                                  captionString:(nullable const char*)caption;

@end

@interface CocoaView : NSView

-(void)setBackingColor:(CGFloat)r
    g:(CGFloat)g
    b:(CGFloat)b
    a:(CGFloat)a;

@end
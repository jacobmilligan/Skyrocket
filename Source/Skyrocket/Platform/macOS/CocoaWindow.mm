//
//  CocoaView.mm
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 6/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//


#include "Skyrocket/Platform/PlatformEvents.hpp"
#include "Skyrocket/Platform/macOS/CocoaWindow.h"

@implementation CocoaView

-(void)
setBackingColor:(CGFloat)r g:(CGFloat) g b:(CGFloat)b a:(CGFloat)a {
// Empty - handled by subclasses
}

@end

@implementation CocoaWindow

-(nonnull instancetype)initWithEventsAndContent:(sky::PlatformEvents * )events
                                    contentRect:(NSRect)contentRect
                                  captionString:(const char *)caption {
    NSUInteger windowStyle = NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask;
    id newWindow = [super initWithContentRect:contentRect
                                styleMask:windowStyle
                                  backing:NSBackingStoreBuffered // double buffered rendering
                                    defer:NO];
    if ( newWindow ) {
        self.events = events;
        NSString* nsTitle;
        if ( caption ) {
            nsTitle = [[[NSString alloc] initWithUTF8String:caption] autorelease];
        } else {
            nsTitle = [[[NSString alloc] initWithUTF8String:"Skyrocket Application"] autorelease];
        }
        [self setTitle:nsTitle];
        [self setAcceptsMouseMovedEvents:YES];
        [self makeKeyAndOrderFront:self];
        [self makeFirstResponder:self];
    }
    return newWindow;
}

-(void)keyDown:(NSEvent * )event {
    _events->key_down([event keyCode]);
}

-(void)keyUp:(NSEvent * )event {
    _events->key_up([event keyCode]);
}

@end

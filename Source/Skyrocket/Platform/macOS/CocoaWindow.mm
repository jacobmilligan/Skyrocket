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


#include "Skyrocket/Platform/Platform.hpp"

#include "Skyrocket/Platform/macOS/CocoaWindow.h"

@implementation CocoaWindow

-(instancetype)initWithSizeAndCaption:(NSRect)contentRect captionString:(const char *)caption {
    
    NSUInteger windowStyle = NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask;
    id newWindow = [super initWithContentRect:contentRect
                                    styleMask:windowStyle
                                      backing:NSBackingStoreBuffered // double buffered rendering
                                        defer:NO];
    if ( newWindow ) {
        NSString* nsTitle;
        if ( caption ) {
            nsTitle = [[[NSString alloc] initWithUTF8String:caption] autorelease];
        } else {
            nsTitle = [[[NSString alloc] initWithUTF8String:"Skyrocket Application"] autorelease];
        }
        
        [self setTitle:nsTitle];
        [self setLevel:NSMainMenuWindowLevel];
        [self makeKeyAndOrderFront:self];
    }
    
    return newWindow;
    
}

@end

@implementation CocoaView

-(void)setBackingColor:(CGFloat)r g:(CGFloat)g b:(CGFloat)b a:(CGFloat)a {
    // Empty - handled by subclasses
}

@end

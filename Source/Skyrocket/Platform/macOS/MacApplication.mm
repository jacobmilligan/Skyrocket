//
//  MacApplication.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 9/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#import "Skyrocket/Platform/macOS/MacApplication.h"

@implementation SkyrocketApplication

// Override sendEvent so that even when a modifier is held at the same time as keyup, it's still sent to
// event queue as individual keys in the combination - specific for games because we don't really need
// combinations that much outside of UI code
-(void)sendEvent:(NSEvent *)event {
    if ( [event type] == NSKeyUp && [event modifierFlags] & NSCommandKeyMask ) {
        [[self keyWindow] sendEvent:event];
    } else {
        [super sendEvent:event];
    }
}

@end

@implementation SkyrocketApplicationDelegate

-(void)applicationDidFinishLaunching:(NSNotification *)notification {
    // Stop the default loop and break out of it with an empty event to control main loop
    [NSApp stop:nil];

    @autoreleasepool {
        NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

        NSEvent* event = [NSEvent otherEventWithType:NSApplicationDefined
                                            location:NSMakePoint(0, 0)
                                       modifierFlags:0
                                           timestamp:0
                                        windowNumber:0
                                             context:nil
                                             subtype:0
                                               data1:0
                                               data2:0];
        [NSApp postEvent:event atStart:YES];
    }
}

@end

//
//  Platform.mm
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by/Users/Jacob/Dev/Repos/SkyrocketModules/Source/Skyrocket/Platform/macOS/CocoaApplication.h
//  Jacob Milligan on 4/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Platform/Platform.hpp"
#include "Skyrocket/Platform/macOS/MacApplication.h"
#include "Skyrocket/Platform/macOS/CocoaWindow.h"
#include "Skyrocket/Core/Diagnostics/Error.hpp"

#include <cstdint>

//---------------------------------------------
// C++ implementations
//---------------------------------------------

namespace sky {

void Platform::native_init()
{
    AssertGuard assert_guard("Initializing platform", app_title_);

//    SKY_ASSERT(app_ != nullptr, "Application is not null");

    // Create shared application and assign app delegate
    [SkyrocketApplication sharedApplication];
    [NSApp setDelegate:[[[SkyrocketApplicationDelegate alloc] init] autorelease]];

    // Set activation policy to regular to avoid requiring .plist files in < OSX 10.7
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

    // Add menu bars
    id menu = [[NSMenu alloc] autorelease];
    id menuItem = [[NSMenuItem new] autorelease];
    [menu addItem:menuItem];

    // Add dropdowns
    id appMenu = [[NSMenu new] autorelease];
    id quitMenuItem = [[[NSMenuItem alloc] initWithTitle:@"Quit"
                                                  action:@selector(terminate:)
                                           keyEquivalent:@"q"] autorelease];
    [appMenu addItem:quitMenuItem];
    [menuItem setSubmenu:appMenu];

    [NSApp setMainMenu:menu];

//    NSArray* args = [[NSProcessInfo processInfo] arguments];
//    int argc = 0;
//    const char* argv[32];
//    for ( NSString* arg in args ) {
//        argv[argc++] = arg.UTF8String;
//        if ( argc == sizeof(argv) / sizeof(argv[0]) ) {
//            break;
//        }
//    }

    initialized_ = true;

    [NSApp run];
    [NSApp activateIgnoringOtherApps:YES]; // make skyrocket the active app

}

void* Platform::create_native_window(const char* caption, const uint16_t width,
                                     const uint16_t height)
{
    NSRect frame = NSMakeRect(0, 0, width, height);
    CocoaWindow * window = [[[CocoaWindow alloc] initWithEventsAndContent:&events_
                                                             contentRect:frame
                                                           captionString:caption] autorelease];
    return window;
}

void Platform::native_poll_events()
{
    @autoreleasepool {

        NSEvent * event;
        while ( true ) {
            event = [NSApp nextEventMatchingMask:NSAnyEventMask
                                       untilDate:[NSDate distantPast]
                                          inMode:NSDefaultRunLoopMode
                                         dequeue:YES];
            if ( event == nil ) {
                break;
            }

            [NSApp sendEvent:event];
        }

    }
}


}

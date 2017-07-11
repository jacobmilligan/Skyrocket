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

#include "Skyrocket/Platform/NativeInput.hpp"
#include "Skyrocket/Platform/Platform.hpp"
#include "Skyrocket/Platform/macOS/MacApplication.h"
#include "Skyrocket/Platform/macOS/CocoaWindow.h"
#include "Skyrocket/Core/Diagnostics/Error.hpp"

#include <cstdint>

//---------------------------------------------
// C++ implementations
//---------------------------------------------

namespace sky {

struct Platform::PlatformHandle {
    NSAutoreleasePool* pool;
    SkyrocketApplicationDelegate* app_delegate;
};


Platform::Platform()
    : handle_(new PlatformHandle)
{
}
    
Platform::~Platform()
{
    if ( handle_ != nullptr ) {
        [handle_->pool drain];
        delete handle_;
    }
    initialized_ = false;
}

void Platform::startup(const char* app_title)
{
    AssertGuard assert_guard("Initializing platform", app_title);
    
//    SKY_ASSERT(app_ != nullptr, "Application is not null");
    
    handle_->pool = [[NSAutoreleasePool alloc] init];
    // Create shared application and assign app delegate
    [SkyrocketApplication sharedApplication];
    handle_->app_delegate = [[SkyrocketApplicationDelegate alloc] init];
    [NSApp setDelegate:handle_->app_delegate];
    
    // Set activation policy to regular to avoid requiring .plist files in < OSX 10.7
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    
    // Add menu bars
    id menu = [[NSMenu alloc] autorelease];
    id menuItem = [[NSMenuItem new] autorelease];
    [menu addItem:menuItem];
    
    // Add dropdowns
    id appMenu = [[NSMenu new] autorelease];
    id quitMenuItem = [[NSMenuItem alloc] initWithTitle:@"Quit"
                                                 action:@selector(terminate:)
                                          keyEquivalent:@"q"];
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

void* Platform::new_native_window(const char* caption, const uint16_t width,
                                  const uint16_t height)
{
    NSRect frame = NSMakeRect(0, 0, width, height);
    CocoaWindow* window = [[CocoaWindow alloc] initWithInputListener:&input_
                                                         contentRect:frame
                                                       captionString:caption];
    return window;
}
    
void Platform::native_poll_event()
{
    NSAutoreleasePool* autoReleasePool = [[NSAutoreleasePool alloc] init];
    
    NSEvent* event;
    while ( true ) {
        event = [NSApp nextEventMatchingMask:NSAnyEventMask
                                   untilDate:[NSDate distantPast]
                                      inMode:NSDefaultRunLoopMode
                                     dequeue:YES];
        if ( event == nil )
            break;
        
        [NSApp sendEvent:event];
    }
    
    [autoReleasePool drain];
}


}

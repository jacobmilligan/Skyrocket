//
//  Keycodes.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by/Users/Jacob/Dev/Repos/SkyrocketModules/Source/Skyrocket/Platform/macOS/CocoaApplication.h
//  Jacob Milligan on 4/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Platform/Platform.hpp"
#include "Skyrocket/Framework/Application.hpp"
#include "Skyrocket/Platform/macOS/CocoaApplication.h"

#include <cstdint>

#import <QuartzCore/QuartzCore.h>
#import <Metal/Metal.h>

//---------------------------------------------
// Skyrocket Application and App
// delegate implementation
//---------------------------------------------

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
    [pool drain];
}

@end

//---------------------------------------------
// Skyrocket View implementation
//---------------------------------------------

@implementation MetalView

-(id)initWithFrame:(NSRect)frameRect {
    self = [super initWithFrame:frameRect];
    
    CAMetalLayer* mtlLayer = [[[CAMetalLayer alloc] init] autorelease];
    
    [self setLayer:mtlLayer];
    
    return self;
}

-(BOOL)acceptsFirstResponder {
    return YES;
}

-(BOOL)acceptsFirstMouse:(NSEvent *)event {
    return YES;
}

-(void)keyDown:(NSEvent *)event {
    auto keycode = [event keyCode];
    self.app->on_key(sky::Platform::get_vk(keycode));
}

@end

//---------------------------------------------
// C++ implementations
//---------------------------------------------

namespace sky {

struct PlatformHandle {
    SkyrocketApplicationDelegate* app_delegate;
    NSWindow* window;
};


sky::Key Platform::keycode_table_[static_cast<uint16_t>(Key::last)];
Application* Platform::app_ = nullptr;

Platform::Platform(Application* app)
    : handle_(new PlatformHandle)
{
    setup_keycodes();
    app_ = app;
}
    
Platform::~Platform()
{
    if ( handle_ != nullptr ) {
        delete handle_;
    }
}

void Platform::setup_keycodes()
{
    keycode_table_[0x00] = sky::Key::A;
    keycode_table_[0x01] = sky::Key::S;
    keycode_table_[0x02] = sky::Key::D;
    keycode_table_[0x03] = sky::Key::F;
    keycode_table_[0x04] = sky::Key::H;
    keycode_table_[0x05] = sky::Key::G;
    keycode_table_[0x06] = sky::Key::Z;
    keycode_table_[0x07] = sky::Key::X;
    keycode_table_[0x08] = sky::Key::C;
    keycode_table_[0x09] = sky::Key::V;
    keycode_table_[0x0B] = sky::Key::B;
    keycode_table_[0x0C] = sky::Key::Q;
    keycode_table_[0x0D] = sky::Key::W;
    keycode_table_[0x0E] = sky::Key::E;
    keycode_table_[0x0F] = sky::Key::R;
    keycode_table_[0x10] = sky::Key::Y;
    keycode_table_[0x11] = sky::Key::T;
    keycode_table_[0x12] = sky::Key::num1;
    keycode_table_[0x13] = sky::Key::num2;
    keycode_table_[0x14] = sky::Key::num3;
    keycode_table_[0x15] = sky::Key::num4;
    keycode_table_[0x16] = sky::Key::num6;
    keycode_table_[0x17] = sky::Key::num5;
    keycode_table_[0x18] = sky::Key::equal;
    keycode_table_[0x19] = sky::Key::num9;
    keycode_table_[0x1A] = sky::Key::num7;
    keycode_table_[0x1B] = sky::Key::minus;
    keycode_table_[0x1C] = sky::Key::num8;
    keycode_table_[0x1D] = sky::Key::num0;
    keycode_table_[0x1E] = sky::Key::right_bracket;
    keycode_table_[0x1F] = sky::Key::O;
    keycode_table_[0x20] = sky::Key::U;
    keycode_table_[0x21] = sky::Key::left_bracket;
    keycode_table_[0x22] = sky::Key::I;
    keycode_table_[0x23] = sky::Key::P;
    keycode_table_[0x25] = sky::Key::L;
    keycode_table_[0x26] = sky::Key::J;
    keycode_table_[0x27] = sky::Key::apostrophe;
    keycode_table_[0x28] = sky::Key::K;
    keycode_table_[0x29] = sky::Key::semicolon;
    keycode_table_[0x2A] = sky::Key::backslash;
    keycode_table_[0x2B] = sky::Key::comma;
    keycode_table_[0x2C] = sky::Key::slash;
    keycode_table_[0x2D] = sky::Key::N;
    keycode_table_[0x2E] = sky::Key::M;
    keycode_table_[0x2F] = sky::Key::period;
    keycode_table_[0x32] = sky::Key::grave_accent;
    keycode_table_[0x41] = sky::Key::keypad_decimal;
    keycode_table_[0x43] = sky::Key::keypad_multiply;
    keycode_table_[0x45] = sky::Key::keypad_plus;
    keycode_table_[0x47] = sky::Key::num_lock;
    keycode_table_[0x4B] = sky::Key::keypad_divide;
    keycode_table_[0x4C] = sky::Key::keypad_enter;
    keycode_table_[0x4E] = sky::Key::keypad_minus;
    keycode_table_[0x51] = sky::Key::keypad_equals;
    keycode_table_[0x52] = sky::Key::keypad_0;
    keycode_table_[0x53] = sky::Key::keypad_1;
    keycode_table_[0x54] = sky::Key::keypad_2;
    keycode_table_[0x55] = sky::Key::keypad_3;
    keycode_table_[0x56] = sky::Key::keypad_4;
    keycode_table_[0x57] = sky::Key::keypad_5;
    keycode_table_[0x58] = sky::Key::keypad_6;
    keycode_table_[0x59] = sky::Key::keypad_7;
    keycode_table_[0x5B] = sky::Key::keypad_8;
    keycode_table_[0x5C] = sky::Key::keypad_9;

// Keys independent of keyboard layout
    keycode_table_[0x24] = sky::Key::enter;
    keycode_table_[0x30] = sky::Key::tab;
    keycode_table_[0x31] = sky::Key::space;
    keycode_table_[0x33] = sky::Key::delete_key;
    keycode_table_[0x35] = sky::Key::escape;
    keycode_table_[0x37] = sky::Key::left_super;
    keycode_table_[0x38] = sky::Key::left_shift;
    keycode_table_[0x39] = sky::Key::caps_lock;
    keycode_table_[0x3A] = sky::Key::left_alt; // osx - left option
    keycode_table_[0x3B] = sky::Key::left_control;
    keycode_table_[0x36] = sky::Key::right_super;
    keycode_table_[0x3C] = sky::Key::right_shift;
    keycode_table_[0x3D] = sky::Key::right_alt;
    keycode_table_[0x3E] = sky::Key::right_control;
// keycode_table_[0x3F] = sky::Key::function;
    keycode_table_[0x40] = sky::Key::f17;
// keycode_table_[0x48] = sky::Key::volumeup;
// keycode_table_[0x49] = sky::Key::volumedown;
// keycode_table_[0x4A] = sky::Key::mute;
    keycode_table_[0x4F] = sky::Key::f18;
    keycode_table_[0x50] = sky::Key::f19;
    keycode_table_[0x5A] = sky::Key::f20;
    keycode_table_[0x60] = sky::Key::f5;
    keycode_table_[0x61] = sky::Key::f6;
    keycode_table_[0x62] = sky::Key::f7;
    keycode_table_[0x63] = sky::Key::f3;
    keycode_table_[0x64] = sky::Key::f8;
    keycode_table_[0x65] = sky::Key::f9;
    keycode_table_[0x67] = sky::Key::f11;
    keycode_table_[0x69] = sky::Key::f13;
    keycode_table_[0x6A] = sky::Key::f16;
    keycode_table_[0x6B] = sky::Key::f14;
    keycode_table_[0x6D] = sky::Key::f10;
    keycode_table_[0x6F] = sky::Key::f12;
    keycode_table_[0x71] = sky::Key::f15;
    keycode_table_[0x72] = sky::Key::insert;
    keycode_table_[0x73] = sky::Key::home;
    keycode_table_[0x74] = sky::Key::page_up;
    keycode_table_[0x75] = sky::Key::delete_key;
    keycode_table_[0x76] = sky::Key::f4;
    keycode_table_[0x77] = sky::Key::end;
    keycode_table_[0x78] = sky::Key::f2;
    keycode_table_[0x79] = sky::Key::page_down;
    keycode_table_[0x7A] = sky::Key::f1;
    keycode_table_[0x7B] = sky::Key::left;
    keycode_table_[0x7C] = sky::Key::right;
    keycode_table_[0x7D] = sky::Key::down;
    keycode_table_[0x7E] = sky::Key::up;
}

void Platform::initialize(const char* app_title)
{
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    // Create shared application and assign app delegate
    [SkyrocketApplication sharedApplication];
    handle_->app_delegate = [[SkyrocketApplicationDelegate alloc] init];
    [NSApp setDelegate:handle_->app_delegate];
    
    // Set activation policy to regular to avoid requiring .plist files in < OSX 10.7
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    
    [NSApp activateIgnoringOtherApps:YES];
    
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

    NSArray* args = [[NSProcessInfo processInfo] arguments];
    int argc = 0;
    const char* argv[32];
    for ( NSString* arg in args ) {
        argv[argc++] = arg.UTF8String;
        if ( argc == sizeof(argv) / sizeof(argv[0]) ) {
            break;
        }
    }
    
    [pool drain];
    
    app_->on_startup(argc, argv);
    
    [NSApp run];
}

uint16_t
Platform::create_window(const char* caption, const int width, const int height)
{
    NSString* nsTitle;
    if ( caption ) {
        nsTitle = [[[NSString alloc] initWithUTF8String:caption] autorelease];
    } else {
        nsTitle = [[[NSString alloc] initWithUTF8String:"Skyrocket Application"] autorelease];
    }
    
    // 3. Create windowrect and associated window with specified style
    NSRect windowRect = NSMakeRect(0, 0, width, height);
    NSUInteger windowStyle = NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask;
    NSWindow* window = [[NSWindow alloc] initWithContentRect:windowRect
                                                   styleMask:windowStyle
                                                     backing:NSBackingStoreBuffered // double buffered rendering
                                                       defer:NO]; // create immediately
    [window autorelease];
    [window setTitle:nsTitle];
    
    MetalView* view = [[[MetalView alloc] initWithFrame:windowRect] autorelease];
    view.app = app_;
    [window setContentView:view];
    [window setLevel:NSMainMenuWindowLevel];
    [window makeKeyAndOrderFront:window];
    
    handle_->window = window;
    
    uint16_t windowNumber = [window windowNumber];
    
    return windowNumber;
}
    
    
bool Platform::open_window_count()
{
    return [[NSApp windows] count];
}
    
void Platform::poll_events()
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

sky::Key Platform::get_vk(const uint16_t native_key)
{
    return keycode_table_[native_key];
}


}

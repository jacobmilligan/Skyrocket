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


#include "Skyrocket/Framework/Application.hpp"
#include "Skyrocket/Platform/Platform.hpp"

#include "Skyrocket/Platform/macOS/CocoaView.h"

@implementation CocoaView

-(id)initWithFrame:(NSRect)frameRect {
    self = [super initWithFrame:frameRect];
    self.app = sky::Platform::application();
    return self;
}

-(void)keyDown:(NSEvent *)event {
    auto keycode = [event keyCode];
    self.app->on_keydown(sky::Platform::get_vk(keycode));
}

-(void)setBackingColor:(CGFloat)r g:(CGFloat)g b:(CGFloat)b a:(CGFloat)a {
    // Not implemented - the only time a CocoaView is used directly is when no graphics
    // API is specified
}


@end

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

#include "Skyrocket/Platform/macOS/macOS.h"
#include "Skyrocket/Platform/Platform.hpp"
#include "Skyrocket/Framework/Application.hpp"

@implementation CocoaView

-(id)initWithFrame:(NSRect)frameRect {
    self = [super initWithFrame:frameRect];
    return self;
}

-(void)keyDown:(NSEvent *)event {
    auto keycode = [event keyCode];
    self.app->on_keydown(sky::Platform::get_vk(keycode));
}


@end

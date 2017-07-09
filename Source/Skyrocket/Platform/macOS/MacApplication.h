//
//  macOSPlatform.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 4/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Platform/Platform.hpp"

#if SKY_OS_MACOS

#include <cstdint>

#import <AppKit/AppKit.h>

@interface SkyrocketApplication : NSApplication
@end

@interface SkyrocketApplicationDelegate : NSObject<NSApplicationDelegate>
@end

#endif

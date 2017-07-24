//
//  MacViewport.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 22/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Platform/macOS/CocoaWindow.h"
#include "Skyrocket/Graphics/Viewport.hpp"


namespace sky {


struct NativeViewport {
    CocoaWindow* window;
    CocoaView* view;
};


}
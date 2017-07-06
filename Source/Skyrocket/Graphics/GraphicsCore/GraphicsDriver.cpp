//
//  GraphicsDriver.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 6/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Graphics/GraphicsCore/GraphicsDriver.hpp"
#include "Skyrocket/Graphics/Viewport.hpp"

namespace sky {


sky::GraphicsDriver::GraphicsDriver(Viewport& viewport)
{
    initialized_ = true;
    viewport_ = &viewport;
}


}
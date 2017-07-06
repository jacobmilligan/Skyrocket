//
//  GraphicsDriver.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 6/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include <memory>


namespace sky {

class Viewport;

class GraphicsDriver {
public:
    GraphicsDriver()
        : initialized_(false)
    {}

    static std::unique_ptr<GraphicsDriver> create(Viewport& viewport);

protected:
    GraphicsDriver(Viewport& viewport);
private:
    bool initialized_;
    Viewport* viewport_;
};


}
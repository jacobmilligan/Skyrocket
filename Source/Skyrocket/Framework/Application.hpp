//
//  Application.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 1/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Input/Keycodes.hpp"

#include <memory>
#include <Skyrocket/Graphics/Core/GraphicsDriver.hpp>

namespace sky {

class Platform;

class Application {
public:
    Application(const char* name);
    ~Application();

    inline const char* app_name()
    {
        return name_;
    }

    uint16_t active_windows();

    void start();
    void shutdown();

    virtual void on_startup(int argc, const char** argv) = 0;
    virtual void on_update() = 0;
    virtual void on_render() = 0;
    virtual void on_keydown(sky::Key keycode) = 0;
    virtual void on_keyup(sky::Key keycode) = 0;
    virtual void on_mouse() = 0;
protected:
    std::unique_ptr<GraphicsDriver> graphics_driver;
private:
    std::unique_ptr<Platform> platform_;
    const char* name_;
    bool active_;

};


}




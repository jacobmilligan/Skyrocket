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

#include "Skyrocket/Graphics/GraphicsDriver.hpp"

namespace sky {

//class Platform;

class Application {
public:
    explicit Application(const char* name);
    ~Application();

    inline const char* app_name()
    {
        return name_;
    }

    inline void set_frame_limit(const double fps)
    {
        target_frametime_ = (1.0 / fps) * 1000.0;
    }

    void start(const GraphicsDriver::ThreadSupport graphics_threading);

    virtual void on_startup(int argc, const char** argv) = 0;
    virtual void on_update() = 0;
    virtual void on_shutdown() = 0;
protected:
    GraphicsDriver graphics_driver;
    Platform platform;
    Viewport primary_view;
private:
    const char* name_;
    bool active_;
    double target_frametime_;

    void shutdown();
};


}  // namespace sky




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

    void start(const GraphicsDriver::ThreadSupport graphics_threading);

    virtual void on_startup(int argc, const char** argv) = 0;
    virtual void on_update() = 0;
    virtual void on_shutdown() = 0;
protected:
    GraphicsDriver graphics_driver;
    Platform platform;
    Viewport primary_view;
private:
    //std::unique_ptr<Platform> platform_;
    const char* name_;
    bool active_;

    uint64_t frame_start = 0;
    sky::Timespan frame_time;

    void shutdown();
};


}  // namespace sky




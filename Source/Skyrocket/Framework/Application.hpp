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

#include "Skyrocket/Platform/Platform.hpp"

#include <memory>

namespace sky {


class Application {
public:
    Application(const char* name);
    ~Application();

    inline const char* app_name()
    {
        return name_;
    }

    void start();
    void shutdown();


    virtual void on_startup(int argc, const char** argv) = 0;
    virtual void on_tick() = 0;
    virtual void on_key(sky::Key keycode) = 0;
    virtual void on_mouse() = 0;
protected:
    std::unique_ptr<Platform> platform_;
private:
    const char* name_;
    bool active_;
};


}




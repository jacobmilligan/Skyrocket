//
//  Application.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 1/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Framework/Application.hpp"
#include "Skyrocket/Platform/Platform.hpp"

namespace sky {


Application::Application(const char* name)
    : name_(name),
      platform_(std::make_unique<Platform>()),
      active_(false)
{}

Application::~Application()
{}

void Application::start()
{
    platform_->startup(name_);

    graphics_driver = GraphicsDriver::create();

    active_ = true;

    while ( active_ ) {
        platform_->poll_events();
        on_update();
        on_render();
        // TODO: only update active view
    }
}

void Application::shutdown()
{
    active_ = false;
}

uint16_t Application::active_windows()
{
    return platform_->open_window_count();
}


}

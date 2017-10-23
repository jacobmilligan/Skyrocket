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

#include <Skyrocket/Graphics/Color.hpp>
#include "Skyrocket/Framework/Application.hpp"

namespace sky {


Application::Application(const char* name)
    : name_(name),
      active_(false)
{}

Application::~Application() = default;

void Application::start(const GraphicsDriver::ThreadSupport graphics_threading)
{
    {
        AssertGuard ag("Application start", name_);

        platform.launch(name_, &Application::on_update);

        primary_view.open(name_, 800, 600);
        primary_view.set_backing_color(sky::Color::gray);

        auto graphics_init_success = graphics_driver.initialize(graphics_threading, &primary_view);

        SKY_ASSERT(graphics_init_success, "GraphicsDriver initialized successfully");

        on_startup(0, nullptr);

        active_ = true;
    }

    auto target_frametime = 16.6;

    while ( Viewport::open_viewports() > 0 ) {
        frame_start = high_resolution_time();
        platform.poll_events();

        on_update();

        frame_time = sky::Timespan(high_resolution_time() - frame_start);

        printf("Frame time: %f\n", frame_time.total_milliseconds());

//        if ( frame_time.total_milliseconds() < target_frametime) {
//            auto diff = target_frametime - frame_time.total_milliseconds();
//            auto sleep_time = sky::Timespan(static_cast<uint64_t>(diff * sky::Timespan::ticks_per_millisecond));
//            sky::thread_sleep(sleep_time);
//        }
    }

    shutdown();
}

void Application::shutdown()
{
    active_ = false;
    on_shutdown();
}


}  // namespace sky

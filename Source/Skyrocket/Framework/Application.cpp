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
      target_frametime_(0.0),
      active_(false)
{
    set_frame_limit(144.0);
}

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

    uint64_t frame_start = 0;
    sky::Timespan frame_time;

    while ( Viewport::open_viewports() > 0 ) {
        frame_start = high_resolution_time();
        platform.poll_events();

        on_update();

        frame_time = sky::Timespan(high_resolution_time() - frame_start);

        printf("Frame time: %f\n", frame_time.total_milliseconds());

        if ( frame_time.total_milliseconds() < target_frametime_ ) {
            auto diff = target_frametime_ - frame_time.total_milliseconds();
            auto sleep_time = sky::Timespan(static_cast<uint64_t>(diff * sky::Timespan::ticks_per_millisecond));
            sky::thread_sleep(sleep_time);
        }

//        printf("Frame time: %f\n", sky::Timespan(high_resolution_time() - frame_start).total_milliseconds());
    }

    shutdown();
}

void Application::shutdown()
{
    active_ = false;
    on_shutdown();
}


}  // namespace sky

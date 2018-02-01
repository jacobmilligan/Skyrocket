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

#include <Jobrocket/Jobrocket.hpp>

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
    AssertGuard ag("Application is starting", name_);
    {
        platform.launch(name_, &Application::on_update);

        primary_view.open(name_, 800, 600);
        primary_view.set_backing_color(sky::Color::gray);

        auto graphics_init_success = graphics_driver.init(graphics_threading, &primary_view);

        SKY_ASSERT(graphics_init_success, "GraphicsDriver initialized successfully");

        if ( graphics_threading == GraphicsDriver::ThreadSupport::multi_threaded ) {
            jobrocket::startup(jobrocket::Scheduler::auto_thread_count, 1);
            SKY_ASSERT(jobrocket::current_scheduler()->num_workers() > 0,
                       "Job scheduler initialized with correct number of workers")
            SKY_ASSERT(jobrocket::current_scheduler()->num_main_threads() == 2,
                       "Job scheduler initialized with simulation and render threads")
        }

        on_startup(0, nullptr);

        active_ = true;
    }

    uint64_t elapsed = 0;

    double num_frames = 0;

    while ( Viewport::open_viewports() > 0 ) {
        elapsed = high_resolution_time();

        platform.poll_events();
        on_update(dt_);

        while (high_resolution_time() - elapsed < target_frametime_) {
            sky::thread_sleep(0);
        }

        dt_ = sky::Timespan(high_resolution_time() - elapsed).total_milliseconds();
        ++num_frames;
    }

    shutdown();
}

void Application::shutdown()
{
    active_ = false;
    on_shutdown();
    jobrocket::shutdown();
}


}  // namespace sky

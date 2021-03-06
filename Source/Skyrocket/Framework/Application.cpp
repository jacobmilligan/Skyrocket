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
      target_frametime_(0),
      active_(false)
{
    set_frame_limit(144.0);
}

Application::~Application() = default;

void Application::start(const Renderer::ThreadSupport graphics_threading,
                        const RendererBackend renderer_backend)
{
    AssertGuard ag("Application is starting", name_);
    {
        platform.launch(name_);

        auto graphics_init_success = renderer.init(graphics_threading, &primary_view, renderer_backend);

        primary_view.open(renderer, name_, 800, 600);

        SKY_ASSERT(graphics_init_success, "Renderer initialized successfully");

        if ( graphics_threading == Renderer::ThreadSupport::multi_threaded ) {
            jobrocket::startup(jobrocket::Scheduler::auto_thread_count, 1);
            SKY_ASSERT(jobrocket::current_scheduler()->num_workers() > 0,
                       "Job scheduler initialized with correct number of workers")
            SKY_ASSERT(jobrocket::current_scheduler()->num_main_threads() == 2,
                       "Job scheduler initialized with simulation and render threads")
        }

        on_startup(0, nullptr);

        active_ = true;
    }

    auto cmdlist = renderer.make_command_list();
    {
        cmdlist.set_clear_color(sky::Color::cornflower_blue);
    }
    renderer.submit(cmdlist);
    renderer.commit_frame();

    uint64_t elapsed = 0;

    double num_frames = 0;

    while ( Viewport::open_viewports() > 0 ) {
        elapsed = high_resolution_time();

        platform.poll_events();
        on_update(dt_);

        if (target_frametime_ > 0) {
            while (high_resolution_time() - elapsed < target_frametime_) {
                sky::thread_sleep(1);
            }
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
    renderer.destroy();
    jobrocket::shutdown();
}

void Application::set_frame_limit(const double fps)
{
    if (fps <= 0.0) {
        target_frametime_ = 0;
        return;
    }

    target_frametime_ = static_cast<uint64_t>(
        Timespan::ticks_per_millisecond * ( (1.0 / fps) * 1000.0 )
    );
}


}  // namespace sky

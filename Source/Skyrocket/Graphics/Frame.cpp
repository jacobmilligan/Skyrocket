//
//  Frame.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 1/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include <cstdio>
#include "Skyrocket/Graphics/Frame.hpp"

namespace sky {

double Frame::running_fps_ = 0.0;
uint64_t Frame::num_frames_ = 0;

void Frame::reset(const uint64_t new_id)
{
    id = new_id;
//    total_fps = 0;
//    total_time = 0.0;
//    cpu_time = 0.0;
//    gpu_time = 0.0;
//    sim_time = 0.0;
//    gdi_time = 0.0;
}

void Frame::begin_frame()
{
    frame_stopwatch_.start();
}

void Frame::end_frame()
{
    frame_stopwatch_.stop();
    total_time = frame_stopwatch_.get_time().total_milliseconds();
    total_fps = Timespan::ticks_per_second / frame_stopwatch_.get_time().ticks();

    if (total_fps <= frame_time_upper_bound_) {
        ++num_frames_;
        running_fps_ += total_fps;
    }
    running_fps_average = running_fps_ / num_frames_;
}

void Frame::sim_begin()
{
    sim_stopwatch_.start();
}

void Frame::sim_end()
{
    sim_stopwatch_.stop();
    sim_time = sim_stopwatch_.get_time().total_milliseconds();
}

void Frame::gdi_begin()
{
    gdi_stopwatch_.start();
}

void Frame::gdi_end()
{
    gdi_stopwatch_.stop();
    gdi_time = gdi_stopwatch_.get_time().total_milliseconds();
}

void Frame::cpu_begin()
{
    cpu_stopwatch_.start();
}

void Frame::cpu_end()
{
    cpu_stopwatch_.stop();
    cpu_time = cpu_stopwatch_.get_time().total_milliseconds();
}

void Frame::gpu_start()
{
    gpu_stopwatch_.start();
}

void Frame::gpu_end()
{
    gpu_stopwatch_.stop();
    gpu_time = gpu_stopwatch_.get_time().total_milliseconds();
}


} // namespace sky
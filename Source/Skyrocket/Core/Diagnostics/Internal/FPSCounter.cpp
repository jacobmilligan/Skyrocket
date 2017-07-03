//
// FPSCounter.cpp
// Skyrocket Framework
//
// ----------------------------------------------------------------------------
//
// Created by Jacob Milligan on 30/10/2016.
// Copyright (c) 2016 Jacob Milligan All rights reserved.
//

#include "Skyrocket/Core/Diagnostics/FPSCounter.hpp"

#include <cstdio>

namespace sky {


FPSCounter::FPSCounter(const uint32_t samples)
    : samples_(samples)
{}

void FPSCounter::start()
{
    watch_.start();
}

void FPSCounter::update()
{
    currentframe_ = watch_.elapsed().seconds();
    dt_ = currentframe_ - lastframe_;
    lastframe_ = currentframe_;
    
    if ( dt_ < 0 ) {
        dt_ = 0;
    }

    frames_++;

    if ( frames_ >= samples_ ) {
        fps_ = accumulated_ / samples_;
        frames_ = 0;
        accumulated_ = 0.0;
    }

    accumulated_ += 1.0 / dt_;
}

double FPSCounter::frames_per_second()
{
    return fps_;
}

double FPSCounter::delta_time()
{
    return dt_;
}

double FPSCounter::elapsed_seconds()
{
    return watch_.elapsed().seconds();
}

uint64_t FPSCounter::elapsed_frames()
{
    return frames_;
}

void FPSCounter::reset()
{
    dt_ = 1.0f / 60.0f;
    currentframe_ = watch_.restart().seconds();
    fps_ = dt_;
    lastframe_ = 0;
    frames_ = 0;
    accumulated_ = dt_;
}
    
    
}

//
// FPSCounter.hpp
// Skyrocket Framework
//
// ----------------------------------------------------------------------------
//
// Created by Jacob Milligan on 30/10/2016.
// Copyright (c) 2016 Jacob Milligan All rights reserved.
//

#pragma once

#include "Stopwatch.hpp"

#include <string>

namespace sky {

/// @brief FPSCounter tracks the current frame time, frames per second over an
/// average number of samples and other time-related diagnostics
class FPSCounter {
public:
    /// @brief Initializes a new counter using the specified number of samples
    /// as the average to use for reporting fps.
    /// @param samples The number of frames to average the total FPS over
    FPSCounter(const uint32_t samples);
    
    void start();
    
    /// @brief Updates the counter and its internal timer
    void update();
    
    /// @brief Gets the frames per second of the last sample
    /// @return Frames per second of the last sample
    double frames_per_second();
    
    /// @brief Gets the time in seconds the last frame took to complete
    /// @return The delta time
    double delta_time();
    
    /// @brief Gets the number of seconds elapsed since the last time the counter
    /// was reset
    /// @return Number of seconds elapsed since last reset
    double elapsed_seconds();
    
    /// @brief Gets the number of frames elapsed since the last time the counter was
    /// reset
    /// @return Number of frames elapsed since last reset
	uint64_t elapsed_frames();
    
    /// @brief Resets the counter to a blank state
    void reset();
    
    /// @brief Gets the maximum number of samples the counter takes before resetting
    /// @return Maximum allowed number of samples
    inline uint32_t samples() { return samples_; }
    inline void set_samplerate(const uint32_t samples)
    {
        samples_ = samples;
        reset();
    }

private:
    /// @brief The number of frames elapsed since last reset
    uint64_t frames_;
    
    /// @brief The internal timer for the counter
    Stopwatch watch_;
    
    /// @brief The timestamp of the current frame
    double currentframe_;
    
    /// @brief The last delta time in fps
    double fps_;
    
    /// @brief The timestamp of the last frame
    double lastframe_;
    
    /// @brief The time it took to complete the last frame
    double dt_;

    /// @brief The accumulated delta time
    double accumulated_;

    /// @brief Number of samples to take before resetting the timer
    uint32_t samples_;
};


}

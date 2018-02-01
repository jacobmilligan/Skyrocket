//
//  Frame.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 1/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Core/Diagnostics/Stopwatch.hpp"

#include <cstdint>

namespace sky {


struct Frame {
    uint64_t id{0};
    double total_fps{0};
    double running_fps_average{0.0};
    double total_time{0.0};
    double cpu_time{0.0};
    double gpu_time{0.0};
    double sim_time{0.0};
    double gdi_time{0.0};

    void reset(uint64_t new_id);

    void begin_frame();
    void end_frame();

    void sim_begin();

    void sim_end();

    void gdi_begin();
    void gdi_end();

    void cpu_begin();

    void cpu_end();

    void gpu_start();

    void gpu_end();
private:
    static constexpr uint64_t frame_time_upper_bound_ = 160;
    static double running_fps_;
    static uint64_t num_frames_;
    Stopwatch frame_stopwatch_, sim_stopwatch_, gdi_stopwatch_, cpu_stopwatch_, gpu_stopwatch_;
};


} // namespace sky
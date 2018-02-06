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

#include "Skyrocket/Graphics/Renderer/Renderer.hpp"
#include "Skyrocket/Platform/Platform.hpp"
#include "Skyrocket/Graphics/Viewport.hpp"

namespace sky {


class Application {
public:
    explicit Application(const char* name);
    ~Application();

    inline const char* app_name()
    {
        return name_;
    }

    void set_frame_limit(const double fps);

    inline uint64_t get_frame_limit() const
    {
        return target_frametime_;
    }

    void start(Renderer::ThreadSupport graphics_threading,
               RendererBackend renderer_backend = RendererBackend::unknown);

    virtual void on_startup(int argc, const char** argv) = 0;
    virtual void on_update(double dt) = 0;
    virtual void on_shutdown() = 0;
protected:
    Renderer renderer;
    Platform platform;
    Viewport primary_view;
private:
    const char* name_;
    bool active_;
    uint64_t target_frametime_;
    double dt_;

    void shutdown();
};


}  // namespace sky




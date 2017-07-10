//
//  main.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 3/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include <Skyrocket/Framework/Application.hpp>
#include <Skyrocket/Platform/Platform.hpp>
#include <Skyrocket/Input/Keyboard.hpp>
#include <Skyrocket/Graphics/Viewport.hpp>
#include <chrono>

class GraphicsApp : public sky::Application {
public:
    GraphicsApp()
        : sky::Application("Skyrocket Graphics Demo")
    {}

    void on_startup(int argc, const char** argv) override
    {
    }

    void on_update() override
    {
        if ( active_windows() <= 0 )
            shutdown();
    }

    void on_render() override
    {

    }

    void on_keydown(sky::Key keycode) override
    {
        if ( keycode == sky::Key::escape ) {
            shutdown();
        }
    }

    void on_mouse() override
    {

    }

    void on_keyup(sky::Key keycode) override
    {
    }
};

int main(int argc, char** argv)
{
//    GraphicsApp app;
//    app.start();

    auto platform = std::make_unique<sky::Platform>();
    auto graphics = std::make_unique<sky::GraphicsDriver>();
    platform->startup("Graphics app");
    auto view = sky::Viewport::create("Graphics App", 800, 600);

    sky::Keyboard keyboard;

    while ( platform->open_window_count() > 0 ) {
        auto now = std::chrono::high_resolution_clock::now();

        platform->poll_events();
        if ( keyboard.key_down(sky::Key::escape) ) {
            break;
        }

        std::chrono::duration<float, std::centi> time = std::chrono::high_resolution_clock::now() - now;
        printf("Time: %f\n", time.count());
    }

    return 0;
}

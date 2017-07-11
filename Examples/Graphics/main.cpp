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
#include <Skyrocket/Core/Diagnostics/Timespan.hpp>

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
	sky::Viewport view;
	view.open("Graphics App", 800, 600);

    sky::Keyboard keyboard;

    sky::Timespan now(sky::Platform::high_resolution_time());
    sky::Timespan after;

    while ( sky::Viewport::open_viewports() > 0 ) {
        now = sky::Platform::high_resolution_time();

        platform->poll_events();
        if ( keyboard.key_down(sky::Key::escape) ) {
            break;
        }

		if ( keyboard.key_typed(sky::Key::space) ) {
			printf("Open windows: %d\n", sky::Viewport::open_viewports());
		}

        after = sky::Platform::high_resolution_time();
    }

    return 0;
}

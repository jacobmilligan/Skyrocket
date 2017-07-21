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
#include <Skyrocket/Graphics/Viewport.hpp>
#include <Skyrocket/Core/Diagnostics/Timespan.hpp>
#include <Skyrocket/Input/Keyboard.hpp>
#include <Skyrocket/Core/Containers/HandleTable.hpp>

#include <cinttypes>

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
    sky::Platform platform;
	platform.launch("Graphics app");
    auto graphics = sky::create_graphics_device_interface();
    graphics->initialize();
	sky::Viewport view;
	view.open("Graphics App", 800, 600);

    sky::Timespan now(sky::high_resolution_time());
    sky::Timespan after;

	sky::Keyboard keyboard;

    sky::HandleTable<int, 23> handles;
    auto id = handles.create(10);
    auto id2 = handles.create(20);
    auto id3 = handles.create(30);
    handles.destroy(id2);
    handles.destroy(id3);

    printf("%d\n", *handles.lookup(id));

    while ( sky::Viewport::open_viewports() > 0 ) {
        now = sky::high_resolution_time();

		platform.poll_events();

		if ( keyboard.key_down(sky::Key::escape) || view.close_requested() ) {
			break;
		}

		if ( keyboard.key_typed(sky::Key::space) ) {
			printf("Open windows: %d\n", sky::Viewport::open_viewports());
		}

		after = sky::high_resolution_time();
    }

    return 0;
}

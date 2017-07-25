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

#include <Skyrocket/Core/Containers/HandleTable.hpp>
#include <Skyrocket/Core/Diagnostics/Timespan.hpp>
#include <Skyrocket/Framework/Application.hpp>
#include <Skyrocket/Input/Keyboard.hpp>
#include <Skyrocket/Graphics/Core/Vertex.hpp>
#include <Skyrocket/Graphics/Renderer.hpp>
#include <Skyrocket/Platform/Filesystem.hpp>

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
	sky::Viewport view;
	view.open("Graphics App", 800, 600);

    sky::Renderer renderer(sky::Renderer::ThreadSupport::multithreaded);

    if ( !renderer.initialize(view) ) {
        SKY_ERROR("Graphics Example", "Couldn't initialize graphics device interface");
        return 0;
    }

    sky::Timespan now(sky::high_resolution_time());
    sky::Timespan dt;

	sky::Keyboard keyboard;

    sky::Vertex vertices[3];
    vertices[0].position = sky::Vector4f(0.0f, 0.5f, 0.0f, 1.0f);
    vertices[1].position = sky::Vector4f(-0.5f, -0.5f, 0.0f, 1.0f);
    vertices[2].position = sky::Vector4f(0.5f, -0.5f, 0.0f, 1.0f);

    vertices[0].color = sky::Vector4f(1.0f, 0.0f, 0.0f, 1.0f);
    vertices[1].color = sky::Vector4f(0.0f, 1.0f, 0.0f, 1.0f);
    vertices[2].color = sky::Vector4f(0.0f, 0.0f, 1.0f, 1.0f);;

    sky::MemoryBlock block;
    block.data = vertices;
    block.size = sizeof(sky::Vertex) * 3;

    auto vbuf_id = renderer.create_vertex_buffer(block, sky::BufferUsage::staticbuf);

//    auto vert_id = renderer.create_shader("basic_vertex");
//    auto frag_id = renderer.create_shader("basic_fragment");

    renderer.set_shaders(0, 0);

    double total = 0.0;
    double frames = 0.0;

    while ( sky::Viewport::open_viewports() > 0 ) {
        now = sky::high_resolution_time();

		platform.poll_events();

		if ( keyboard.key_down(sky::Key::escape) || view.close_requested() ) {
			break;
		}

		if ( keyboard.key_typed(sky::Key::space) ) {
			printf("Open windows: %d\n", sky::Viewport::open_viewports());
		}

        renderer.set_vertex_buffer(vbuf_id);
        renderer.present();

		dt = sky::high_resolution_time() - now.ticks();

        printf("%f\r", dt.total_seconds());
        total += dt.total_seconds();
        frames++;
    }

    printf("Average: %f\n", total / frames);

    return 0;
}

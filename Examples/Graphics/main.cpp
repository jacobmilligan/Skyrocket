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
#include <Skyrocket/Graphics/Core/Vertex.hpp>
#include <Skyrocket/Graphics/GraphicsDriver.hpp>
#include <Skyrocket/Input/Keyboard.hpp>
#include <Skyrocket/Platform/Filesystem.hpp>

#include <Skyrocket/Core/Math.hpp>
#include <Skyrocket/Graphics/Color.hpp>
#include <iostream>

struct Rectangle {
    Rectangle()
    {
        vertices = {
            sky::Vertex(-1.0f, 1.0f, 0.0f, 1, 0, 1, 1, 1),
            sky::Vertex(1.0f, 1.0f, 0.0f, 1, 0, 0, 1, 1),
            sky::Vertex(1.0f, -1.0f, 0.0f, 1, 1, 0, 1, 1),
            sky::Vertex(-1.0f, -1.0f, 0.0f, 1, 1, 1, 1, 1)
        };

        indices = {
            0, 1, 3,
            3, 2, 1
        };
    }

    sky::Matrix4f model;
    std::vector<sky::Vertex> vertices;
    std::vector<uint32_t> indices;
};

struct ModelViewProjection {
    ModelViewProjection() = default;
    sky::Matrix4f model_view;
    sky::Matrix4f projection;
};

int main(int argc, char** argv)
{
    const char* app_name = "Skyrocket Graphics Example";
    sky::Platform platform;
	platform.launch(app_name);
	sky::Viewport view;
	view.open(app_name, 800, 600);
    view.set_backing_color(sky::Color::gray);

    sky::GraphicsDriver renderer(sky::GraphicsDriver::ThreadSupport::single_thread);

    if ( !renderer.initialize(view) ) {
        SKY_ERROR(app_name, "Couldn't initialize graphics device interface");
        return 0;
    }

	sky::Keyboard keyboard;

    Rectangle rect;

    auto vbuf_id = renderer.create_vertex_buffer(sky::MemoryBlock {
        static_cast<uint32_t>(sizeof(sky::Vertex) * rect.vertices.size()),
        rect.vertices.data()
    }, sky::BufferUsage::staticbuf);
    auto ibuf_id = renderer.create_index_buffer(sky::MemoryBlock {
        static_cast<uint32_t>(sizeof(uint32_t) * rect.indices.size()),
        rect.indices.data()
    });
    auto ubuf_id = renderer.create_uniform(sky::UniformType::mat4, 2);

    ModelViewProjection mvp;

    sky::Matrix4f identity;

    double total = 0;
    double frames = 0.0;

    sky::Vector3f pos;
    sky::Vector3f scale(10.0f, 10.0f, 0.0f);

    auto translation_mat = identity.translate(pos);
    auto scale_mat = identity.scale(scale);
    mvp.model_view = translation_mat * scale_mat;
    mvp.projection = identity.ortho(0.0f, view.size().x, view.size().y, 0.0f, -1.0f, 1.0f);

    renderer.update_uniform(ubuf_id, sky::MemoryBlock{ sizeof(ModelViewProjection), &mvp});

    renderer.set_shaders(0, 0);

    auto dt = 0.0;
    uint64_t frame_start = 0;

    while ( sky::Viewport::open_viewports() > 0 ) {
        frame_start = sky::high_resolution_time();

		platform.poll_events();

		if ( keyboard.key_down(sky::Key::escape) || view.close_requested() ) {
			break;
		}

		if ( keyboard.key_typed(sky::Key::space) ) {
			printf("Open windows: %d\n", sky::Viewport::open_viewports());
		}

        if ( keyboard.key_down(sky::Key::down) ) {
            pos.y += 0.2f * dt;
        }
        if ( keyboard.key_down(sky::Key::right) ) {
            pos.x += 0.2f * dt;
        }
        if ( keyboard.key_down(sky::Key::up) ) {
            pos.y -= 0.2f * dt;
        }
        if ( keyboard.key_down(sky::Key::left) ) {
            pos.x -= 0.2f * dt;
        }

        translation_mat = identity.translate(pos);
        scale_mat = identity.scale(scale);
        mvp.model_view = translation_mat * scale_mat;

        renderer.update_uniform(ubuf_id, sky::MemoryBlock{ sizeof(ModelViewProjection), &mvp});
        renderer.set_vertex_buffer(vbuf_id, 0, static_cast<uint32_t>(rect.vertices.size()));
        renderer.set_uniform(ubuf_id, 1);
        renderer.set_index_buffer(ibuf_id, 0, static_cast<uint32_t>(rect.indices.size()));
        renderer.draw_primitives();
        renderer.present(16.0f);


        dt = sky::Timespan(sky::high_resolution_time() - frame_start).total_milliseconds();
        printf("%f\n", dt);
    }

//    printf("Average: %fms STD: %fms\n", total / frames, std_total / frames);

    return 0;
}

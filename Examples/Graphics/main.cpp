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
#include <Skyrocket/Core/Math.hpp>
#include <Skyrocket/Graphics/Color.hpp>
#include <Skyrocket/Graphics/Core/Vertex.hpp>
#include <Skyrocket/Graphics/Font.hpp>
#include <Skyrocket/Graphics/GraphicsDriver.hpp>
#include <Skyrocket/Input/Keyboard.hpp>

#include <iostream>

struct Cube {
    sky::Vector3f pos;
    sky::Vector3f scale;
    sky::Vector2f angle;

    Cube(sky::GraphicsDriver* driver)
    {
        auto cube_width = 1.0f;
        auto cube_height = 1.0f;
        auto cube_depth = 1.0f;
        vertices_ = {
            // 0
            sky::Vertex(-cube_width,-cube_height,-cube_depth, 1.0f, 0.0f,1.0f,1.0f,1.0f),
            //1
            sky::Vertex( cube_width,-cube_height,-cube_depth, 1.0f, 0.0f,0.0f,1.0f,1.0f),
            //2
            sky::Vertex( cube_width, cube_height,-cube_depth, 1.0f, 1.0f,0.0f,1.0f,1.0f),
            //3
            sky::Vertex(-cube_width, cube_height,-cube_depth, 1.0f, 1.0f,1.0f,1.0f,1.0f),
            //4
            sky::Vertex(-cube_width,-cube_height, cube_depth, 1.0f, 0.0f,1.0f,0.0f,1.0f),
            //5
            sky::Vertex( cube_width,-cube_height, cube_depth, 1.0f, 0.0f,0.0f,0.0f,1.0f),
            //6
            sky::Vertex( cube_width, cube_height, cube_depth, 1.0f, 1.0f,0.0f,0.0f,1.0f),
            //7
            sky::Vertex(-cube_width, cube_height, cube_depth, 1.0f, 1.0f,1.0f,0.0f,1.0f)
        };

        indices_ = {
            0, 1, 3, 3, 1, 2,
            1, 5, 2, 2, 5, 6,
            5, 4, 6, 6, 4, 7,
            4, 0, 7, 7, 0, 3,
            3, 2, 7, 7, 2, 6,
            4, 5, 0, 0, 5, 1
        };

        vbuf_id_ = driver->create_vertex_buffer(sky::MemoryBlock {
            static_cast<uint32_t>(sizeof(sky::Vertex) * vertices_.size()),
            vertices_.data()
        }, sky::BufferUsage::staticbuf);
        ibuf_id_ = driver->create_index_buffer(sky::MemoryBlock {
            static_cast<uint32_t>(sizeof(uint32_t) * indices_.size()),
            indices_.data()
        });

        xaxis_ = sky::Vector3f(0.0f, 1.0f, 0.0f);
        yaxis_ = sky::Vector3f(1.0f, 0.0f, 0.0f);
        pos = sky::Vector3f(0.0f, 0.0f, 0.0f);
        scale = sky::Vector3f(100.0f, 100.0f, 100.0f);
    }

    sky::Matrix4f get_transform()
    {
        auto translation_mat = identity_.translate(pos);
        auto scale_mat = identity_.scale(scale);
        auto rotation_mat = identity_.rotate(angle.x, xaxis_) * identity_.rotate(angle.y, yaxis_);

        return translation_mat * rotation_mat * scale_mat;
    }

    void render()
    {
        driver_->set_vertex_buffer(vbuf_id_, 0, static_cast<uint32_t>(vertices_.size()));
        driver_->set_index_buffer(ibuf_id_, 0, static_cast<uint32_t>(indices_.size()));
        driver_->draw_primitives();
    }

private:
    sky::GraphicsDriver* driver_;
    std::vector<sky::Vertex> vertices_;
    std::vector<uint32_t> indices_;
    uint32_t vbuf_id_, ibuf_id_;
    sky::Vector3f xaxis_, yaxis_;

    sky::Matrix4f identity_;
};

struct ModelViewProjection {
    ModelViewProjection() = default;
    sky::Matrix4f model_view;
    sky::Matrix4f projection;
};

int main(int argc, char** argv)
{
    // Setup paths and string data
    const char* app_name = "Skyrocket Graphics Example";
    auto root_path = sky::Path::executable_path().relative_path("../../../../Examples/Graphics");
    root_path.make_real();

    // Skyrocket platform and input handling data
    sky::Platform platform;
    sky::Viewport view;
    sky::Keyboard keyboard;

    platform.launch(app_name);
	view.open(app_name, 800, 600);
    view.set_backing_color(sky::Color::gray);
    sky::GraphicsDriver driver(sky::GraphicsDriver::ThreadSupport::single_thread);

    if ( !driver.initialize(view) ) {
        SKY_ERROR(app_name, "Couldn't initialize graphics device interface");
        return 0;
    }

    // Geometry
    Cube cube(&driver);
    ModelViewProjection mvp;
    auto angle_x = 90.0f;
    auto angle_y = 0.0f;

    auto cam_pos = sky::Vector3f(0.0f, 0.0f, 250.0f);
    auto cam_front = sky::Vector3f(0.0f, 0.0f, -1.0f);
    auto cam_up = sky::Vector3f(0.0f, 1.0f, 0.0f);

    sky::Matrix4f identity;
    auto view_mat = identity.look_at(cam_pos, cam_pos + cam_front, cam_up);
    mvp.model_view = view_mat * cube.get_transform();
    mvp.projection = identity.perspective(sky::math::to_radians(90.0f), view.size().x / view.size().y, 0.1f, 5000.0f);

    // Graphics resources
    sky::Font font;
    font.load_from_file(root_path.relative_path("Go-Regular.ttf"), 32);

    auto ubuf_id = driver.create_uniform(sky::UniformType::mat4, 2);

    driver.update_uniform(ubuf_id, sky::MemoryBlock{ sizeof(ModelViewProjection), &mvp});
    driver.set_shaders(0, 0);

    // Main loop
    sky::Timespan dt;
    uint64_t frame_start = 0;
    auto cam_speed = 10.0f;
    auto target_frametime = 16.6;

    while ( sky::Viewport::open_viewports() > 0 ) {
        frame_start = sky::high_resolution_time();

		platform.poll_events();

		if ( keyboard.key_down(sky::Key::escape) || view.close_requested() ) {
            break;
		}

        if ( keyboard.key_down(sky::Key::up) ) {
            cam_pos += (cam_front * cam_speed);
        }
        if ( keyboard.key_down(sky::Key::down) ) {
            cam_pos -= (cam_front * cam_speed);
        }
        if ( keyboard.key_down(sky::Key::left) ) {
            cam_pos -= (cam_front.cross(cam_up) * cam_speed);
        }
        if ( keyboard.key_down(sky::Key::right) ) {
            cam_pos += (cam_front.cross(cam_up) * cam_speed);
        }

        cube.angle += 0.2f;

        view_mat = identity.look_at(cam_pos, cam_pos + cam_front, cam_up);
        mvp.model_view = view_mat * cube.get_transform();
        driver.update_uniform(ubuf_id, sky::MemoryBlock{ sizeof(ModelViewProjection), &mvp});
        driver.set_uniform(ubuf_id, 1);

        cube.render();

        driver.present();

        dt = sky::Timespan(sky::high_resolution_time() - frame_start);

        if ( dt.total_milliseconds() < target_frametime) {
            auto diff = target_frametime - dt.total_milliseconds();
            auto sleep_time = sky::Timespan(static_cast<uint64_t>(diff * sky::Timespan::ticks_per_millisecond));
            sky::thread_sleep(sleep_time);
        }
    }

    return 0;
}

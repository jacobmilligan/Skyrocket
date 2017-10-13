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
#include <Skyrocket/Graphics/Core/GraphicsData.hpp>
#include <Skyrocket/Graphics/Font.hpp>
#include <Skyrocket/Graphics/GraphicsDriver.hpp>
#include <Skyrocket/Input/Keyboard.hpp>

#include <iostream>
#include <Skyrocket/Core/Hash.hpp>

struct Cube {
    sky::Vector3f pos;
    sky::Vector3f scale;
    sky::Vector2f angle;

    Cube() = default;

    void init(sky::GraphicsDriver* driver)
    {
        driver_ = driver;

        auto cube_width = 1.0f;
        auto cube_height = 1.0f;
        auto cube_depth = 1.0f;
        vertices_ = {
            sky::GraphicsData(-cube_width,-cube_height,-cube_depth, 1.0f, 0.0f,1.0f,1.0f,1.0f),
            sky::GraphicsData( cube_width,-cube_height,-cube_depth, 1.0f, 0.0f,0.0f,1.0f,1.0f),
            sky::GraphicsData( cube_width, cube_height,-cube_depth, 1.0f, 1.0f,0.0f,1.0f,1.0f),
            sky::GraphicsData(-cube_width, cube_height,-cube_depth, 1.0f, 1.0f,1.0f,1.0f,1.0f),
            sky::GraphicsData(-cube_width,-cube_height, cube_depth, 1.0f, 0.0f,1.0f,0.0f,1.0f),
            sky::GraphicsData( cube_width,-cube_height, cube_depth, 1.0f, 0.0f,0.0f,0.0f,1.0f),
            sky::GraphicsData( cube_width, cube_height, cube_depth, 1.0f, 1.0f,0.0f,0.0f,1.0f),
            sky::GraphicsData(-cube_width, cube_height, cube_depth, 1.0f, 1.0f,1.0f,0.0f,1.0f)
        };

        indices_ = {
            0, 1, 3, 3, 1, 2,
            1, 5, 2, 2, 5, 6,
            5, 4, 6, 6, 4, 7,
            4, 0, 7, 7, 0, 3,
            3, 2, 7, 7, 2, 6,
            4, 5, 0, 0, 5, 1
        };

        auto vertices_size = static_cast<uint32_t>(sizeof(sky::GraphicsData) * vertices_.size());
        auto indices_size = static_cast<uint32_t>(sizeof(uint32_t) * indices_.size());

        auto vbuf_mem = sky::MemoryBlock { vertices_size, vertices_.data() };
        auto ibuf_mem = sky::MemoryBlock { indices_size, indices_.data() };

        vbuf_id_ = driver->create_vertex_buffer(vbuf_mem, sky::BufferUsage::staticbuf);
        ibuf_id_ = driver->create_index_buffer(ibuf_mem);

        xaxis_ = sky::Vector3f(0.0f, 1.0f, 0.0f);
        yaxis_ = sky::Vector3f(1.0f, 0.0f, 0.0f);
        pos = sky::Vector3f(0.0f, 0.0f, 0.0f);
        scale = sky::Vector3f(100.0f, 100.0f, 100.0f);
    }

    sky::Matrix4f& get_transform()
    {
        auto translation_mat = identity_.translate(pos);
        auto scale_mat = identity_.scale(scale);
        auto rotation_mat = identity_.rotate(angle.x, xaxis_) * identity_.rotate(angle.y, yaxis_);

        transform_ = translation_mat * rotation_mat * scale_mat;

        return transform_;
    }

    void render()
    {
        auto vertices_size = static_cast<uint32_t>(vertices_.size());
        auto indices_size = static_cast<uint32_t>(indices_.size());

        driver_->set_vertex_buffer(vbuf_id_, 0, vertices_size);
        driver_->set_index_buffer(ibuf_id_, 0, indices_size);
        driver_->draw_primitives();
    }

private:
    sky::GraphicsDriver* driver_{};
    std::vector<sky::GraphicsData> vertices_;
    std::vector<uint32_t> indices_;
    uint32_t vbuf_id_{}, ibuf_id_{};
    sky::Vector3f xaxis_, yaxis_;

    sky::Matrix4f identity_, transform_;
};

struct ModelViewProjection {
    ModelViewProjection() = default;
    sky::Matrix4f model_view;
    sky::Matrix4f projection;
};

int main(int argc, char** argv)
{
    // Setup paths and string data
    const char* app_name = "Skyrocket Cubes Example";
    auto root_path = sky::Path::executable_path().relative_path("../../../../Examples/Cubes");
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

    auto vert_path = root_path.relative_path("basic_vertex.metal");
    auto frag_path = root_path.relative_path("basic_fragment.metal");
    auto prog = driver.create_program(vert_path, frag_path);
    driver.set_program(prog);

    std::array<Cube, 3> cubes;
    auto pos = 0.0f;
    for ( auto& c : cubes ) {
        c.init(&driver);
        c.pos = sky::Vector3f(pos, pos, 1.0f);
        pos += 100.0f;
    }

    sky::Matrix4f identity, view_mat;
    auto fovy = static_cast<float>(sky::math::to_radians(90.0f));

    auto cam_pos = sky::Vector3f(0.0f, 0.0f, 250.0f);
    auto cam_front = sky::Vector3f(0.0f, 0.0f, -1.0f);
    auto cam_up = sky::Vector3f(0.0f, 1.0f, 0.0f);

    auto projection_mat = identity.perspective(fovy, view.size().x / view.size().y, 0.1f, 5000.0f);
    auto model_ubuf = driver.create_uniform(sky::UniformType::mat4, 1);
    auto view_ubuf = driver.create_uniform(sky::UniformType::mat4, 1);
    auto projection_ubuf = driver.create_uniform(sky::UniformType::mat4, 1);

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

        view_mat = identity.look_at(cam_pos, cam_pos + cam_front, cam_up);

        driver.update_uniform(view_ubuf, sky::MemoryBlock{ sizeof(sky::Matrix4f), &view_mat});
        driver.update_uniform(projection_ubuf, sky::MemoryBlock{ sizeof(sky::Matrix4f), &projection_mat});
        driver.set_uniform(view_ubuf, 2);
        driver.set_uniform(projection_ubuf, 3);

        for ( auto& c : cubes ) {
            driver.set_uniform(model_ubuf, 1);
            c.angle += 0.02f;
            driver.update_uniform(model_ubuf, sky::MemoryBlock{ sizeof(sky::Matrix4f), &c.get_transform()});
            c.render();
        }

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

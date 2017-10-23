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
#include <Skyrocket/Core/Hash.hpp>
#include <Skyrocket/Core/Math.hpp>
#include <Skyrocket/Graphics/Color.hpp>
#include <Skyrocket/Graphics/GraphicsDriver.hpp>
#include <Skyrocket/Graphics/Vertex.hpp>
#include <Skyrocket/Input/Keyboard.hpp>
#include <Skyrocket/Resource/Font.hpp>

#include <Skyrocket/Framework/Application.hpp>

#include <iostream>
#include <random>


struct Cube {
    sky::Vector3f pos;
    sky::Vector3f scale;
    sky::Vector2f angle;

    Cube() = default;

    void init(sky::GraphicsDriver* driver)
    {
        driver_ = driver;

        //Front
        auto a = sky::Vertex(-1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.25f, 0.25f);
        auto b = sky::Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.25f, 0.50f);
        auto c = sky::Vertex( 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.50f, 0.50f);
        auto d = sky::Vertex( 1.0f,  1.0f,  1.0f, 1.0f, 0.1f, 0.6f, 0.4f, 1.0f, 0.50f, 0.25f);
        //Left
        auto e = sky::Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.00f, 0.25f);
        auto f = sky::Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.00f, 0.50f);
        auto g = sky::Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.25f, 0.50f);
        auto h = sky::Vertex(-1.0f,  1.0f,  1.0f, 1.0f, 0.1f, 0.6f, 0.4f, 1.0f, 0.25f, 0.25f);
        //Right
        auto i = sky::Vertex( 1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.50f, 0.25f);
        auto j = sky::Vertex( 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.50f, 0.50f);
        auto k = sky::Vertex( 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.75f, 0.50f);
        auto l = sky::Vertex( 1.0f,  1.0f, -1.0f, 1.0f, 0.1f, 0.6f, 0.4f, 1.0f, 0.75f, 0.25f);
        //Top
        auto m = sky::Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.25f, 0.00f);
        auto n = sky::Vertex(-1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.25f, 0.25f);
        auto o = sky::Vertex( 1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.50f, 0.25f);
        auto p = sky::Vertex( 1.0f,  1.0f, -1.0f, 1.0f, 0.1f, 0.6f, 0.4f, 1.0f, 0.50f, 0.00f);
        //Bottom
        auto q = sky::Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.25f, 0.50f);
        auto r = sky::Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.25f, 0.75f);
        auto s = sky::Vertex( 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.50f, 0.75f);
        auto t = sky::Vertex( 1.0f, -1.0f,  1.0f, 1.0f, 0.1f, 0.6f, 0.4f, 1.0f, 0.50f, 0.50f);
        //Back
        auto u = sky::Vertex( 1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.75f, 0.25f);
        auto v = sky::Vertex( 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.75f, 0.50f);
        auto w = sky::Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.00f, 0.50f);
        auto x = sky::Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.1f, 0.6f, 0.4f, 1.0f, 1.00f, 0.25f);

        vertices_ = {
            a, b, c, a, c, d,   //front
            e, f, g, e, g, h,   //left
            i, j, k, i, k, l,   //right
            m, n, o, m, o, p,   //top
            q, r, s, q, s, t,   //bot
            u, v, w, u, w, x    //back
        };

        auto vertices_size = static_cast<uint32_t>(sizeof(sky::Vertex) * vertices_.size());

        auto vbuf_mem = sky::MemoryBlock { vertices_size, vertices_.data() };

        vbuf_id_ = driver->create_vertex_buffer(vbuf_mem, sky::BufferUsage::staticbuf);

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

        driver_->set_vertex_buffer(vbuf_id_, 0, vertices_size);
        driver_->draw_primitives();
    }

private:
    sky::GraphicsDriver* driver_{};
    std::vector<sky::Vertex> vertices_;
    uint32_t vbuf_id_{};
    sky::Vector3f xaxis_, yaxis_;

    sky::Matrix4f identity_, transform_;
};

class CubeApp : public sky::Application {
public:
    CubeApp()
        : sky::Application("Skyrocket Cubes Example"),
          rand_gen(rd()),
          neg_dist(0.0f, 1.0f),
          dist(0.04f, 0.07f),
          big_dist(-300.0f, 300.0f),
          cam_pos_(0.0f, 0.0f, 250.0f),
          cam_front_(0.0f, 0.0f, -1.0f),
          cam_up_(0.0f, 1.0f, 0.0f)
    {
        root_path_ = sky::Path::executable_path().relative_path("../../../../Examples/Cubes");
        if ( sky::target_platform == sky::OS::macos ) {
            root_path_ = sky::Path("/Users/Jacob/Dev/Repos/Skyrocket/Examples/Cubes");
        }
        root_path_.make_real();
    }

    void on_startup(int argc, const char** argv) override
    {
        auto vert_path = root_path_.relative_path("basic_vertex.metal");
        auto frag_path = root_path_.relative_path("basic_fragment.metal");
        program_ = graphics_driver.create_program(vert_path, frag_path);
        graphics_driver.set_program(program_);

        auto pos = 0.0f;
        for ( auto& c : cubes ) {
            c.init(&graphics_driver);
            c.pos = sky::Vector3f(pos + big_dist(rand_gen), pos + big_dist(rand_gen), pos + big_dist(rand_gen));
            pos += 100.0f;
        }

        sky::Matrix4f identity, view_mat;
        auto fovy = static_cast<float>(sky::math::to_radians(90.0f));

        auto aspect = primary_view.size().x / primary_view.size().y;
        projection_mat_ = identity.perspective(fovy, aspect, 0.1f, 5000.0f);

        model_ubuf_ = graphics_driver.create_uniform(sky::UniformType::mat4, 1);
        view_ubuf_ = graphics_driver.create_uniform(sky::UniformType::mat4, 1);
        projection_ubuf_ = graphics_driver.create_uniform(sky::UniformType::mat4, 1);

        // Main loop
        sky::Timespan dt;
        uint64_t frame_start = 0;
        auto cam_speed = 10.0f;
        auto target_frametime = 16.6;

        sky::Image cube_tex;
        cube_tex.load_from_file(root_path_.relative_path("cube.png"));

        texture_ = graphics_driver.create_texture(cube_tex);
    }

    void on_update() override
    {
        graphics_driver.set_state(sky::RenderPipelineState::culling_frontface);

        if ( keyboard_.key_down(sky::Key::escape) || primary_view.close_requested() ) {
            primary_view.close();
        }

        if ( keyboard_.key_down(sky::Key::up) ) {
            cam_pos_ += (cam_front_ * cam_speed_);
        }
        if ( keyboard_.key_down(sky::Key::down) ) {
            cam_pos_ -= (cam_front_ * cam_speed_);
        }
        if ( keyboard_.key_down(sky::Key::left) ) {
            cam_pos_ -= (cam_front_.cross(cam_up_) * cam_speed_);
        }
        if ( keyboard_.key_down(sky::Key::right) ) {
            cam_pos_ += (cam_front_.cross(cam_up_) * cam_speed_);
        }

        view_mat_ = identity_.look_at(cam_pos_, cam_pos_ + cam_front_, cam_up_);

        graphics_driver.update_uniform(view_ubuf_, sky::MemoryBlock{ sizeof(sky::Matrix4f), &view_mat_});
        graphics_driver.update_uniform(projection_ubuf_, sky::MemoryBlock{ sizeof(sky::Matrix4f), &projection_mat_});
        graphics_driver.set_uniform(view_ubuf_, 2);
        graphics_driver.set_uniform(projection_ubuf_, 3);
        graphics_driver.set_texture(texture_, 0);

        for ( auto& c : cubes ) {
            graphics_driver.set_uniform(model_ubuf_, 1);
            c.angle += dist(rand_gen);

            sky::MemoryBlock mb {
                sizeof(sky::Matrix4f),
                &c.get_transform()
            };

            graphics_driver.update_uniform(model_ubuf_, mb);
            c.render();
        }

        graphics_driver.present();
    }

    void on_shutdown() override
    {

    }

private:
    sky::Viewport view_;
    sky::Keyboard keyboard_;

    sky::Path root_path_;

    std::random_device rd;
    std::mt19937 rand_gen;
    std::uniform_real_distribution<float> neg_dist;
    std::uniform_real_distribution<float> dist;
    std::uniform_real_distribution<float> big_dist;

    sky::Vector3f cam_pos_, cam_front_, cam_up_;
    sky::Matrix4f identity_, view_mat_, projection_mat_;
    float cam_speed_{10.0f};

    uint32_t program_{}, model_ubuf_{}, projection_ubuf_{}, view_ubuf_{}, texture_{};
    std::array<Cube, 3> cubes;
};

int main(int argc, char** argv)
{
    CubeApp app;
    app.start(sky::GraphicsDriver::ThreadSupport::multithreaded);

    return 0;
}

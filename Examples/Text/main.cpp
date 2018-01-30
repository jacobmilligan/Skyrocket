//
//  main.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 27/10/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include <Skyrocket/Framework/Application.hpp>
#include <Skyrocket/Input/Keyboard.hpp>
#include <Skyrocket/Resource/Font.hpp>
#include <Skyrocket/Graphics/Vertex.hpp>
#include <Skyrocket/Framework/Camera.hpp>

class TextApplication : public sky::Application {
public:
    TextApplication()
        : Application("Skyrocket Text Rendering Example")
    {
        root_path_ = sky::Path::executable_path().relative_path("../../../../Examples/Text");
        if ( sky::target_platform == sky::OS::macos ) {
            root_path_ = sky::Path("/Users/Jacob/Dev/Repos/Skyrocket/Examples/Text");
        }
        root_path_.make_real();
    }

    void on_startup(int argc, const char** argv) override
    {
        cam_speed_ = 4.5f;
        cam_.setup(primary_view.size(), 0.01f, 1000.0f);

        primary_view.set_backing_color(sky::Color::cornflower_blue);

        font_.load_from_file(root_path_.relative_path("Go-Regular.ttf"), 23);
//        font_.load_from_file(root_path_.relative_path("Arial.ttf"), 23);

        auto cmdqueue = graphics_driver.command_list();
        tex_ = cmdqueue->create_texture(font_.width(), font_.height(),
                                      sky::PixelFormat::Enum::r8);

        auto padding = 0;
        auto row = 0;
        for ( auto& g : font_ ) {
            if ( g.bounds.width <= 0 || g.bounds.height <= 0 || g.data == nullptr ) {
                continue;
            }
            cmdqueue->create_texture_region(tex_, g.bounds, sky::PixelFormat::Enum::r8, g.data);
        }

        std::string str = "Hello World!";
        auto x = 0.0f;
        auto y = 0.0f;

        sky::Glyph glyph;
        for ( auto& c : str ) {
            glyph = font_.get_glyph(c);
            auto& bounds = glyph.bounds;

            auto ypos = y - (glyph.bounds.height - glyph.bearing.y);
            auto xpos = x;

            vertices_.emplace_back(xpos, ypos + bounds.height, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, glyph.s, glyph.t);
            vertices_.emplace_back(xpos, ypos, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, glyph.s, glyph.t2);
            vertices_.emplace_back(xpos + bounds.width, ypos, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, glyph.s2, glyph.t2);

            vertices_.emplace_back(xpos, ypos + bounds.height, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, glyph.s, glyph.t);
            vertices_.emplace_back(xpos + bounds.width, ypos, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, glyph.s2, glyph.t2);
            vertices_.emplace_back(xpos + bounds.width, ypos + bounds.height, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, glyph.s2, glyph.t);

            x += glyph.advance.x;
        }

        cam_.set_position({0.0f, 0.0f});

        auto mb = sky::alloc(sizeof(sky::Vertex) * vertices_.size(), vertices_.data());
        vbuf_ = cmdqueue->create_vertex_buffer(mb, sky::BufferUsage::staticbuf);
        viewproj_ = cmdqueue->create_uniform(sky::UniformType::mat4, sizeof(sky::Matrix4f));
        cmdqueue->set_vertex_buffer(vbuf_, 0, static_cast<uint32_t>(vertices_.size()));

        auto vert_path = root_path_.relative_path("basic_vertex.metal");
        auto frag_path = root_path_.relative_path("basic_fragment.metal");
        program_ = cmdqueue->create_program(vert_path, frag_path);
        cmdqueue->set_program(program_);
        graphics_driver.commit_command_list();
    }

    void on_update() override
    {
        cam_movement_.x = 0.0f;
        cam_movement_.y = 0.0f;

        if ( keyboard_.key_typed(sky::Key::escape) ) {
            primary_view.close();
        }

        auto cmdqueue = graphics_driver.command_list();

        cmdqueue->set_state(sky::RenderPipelineState::culling_frontface);

        cmdqueue->set_program(program_);
        cmdqueue->set_uniform(viewproj_, 1);
        cmdqueue->set_vertex_buffer(vbuf_, 0, static_cast<uint32_t>(vertices_.size()));
        cmdqueue->set_texture(tex_, 0);

        if (keyboard_.key_down(sky::Key::down)) {
            cam_movement_.y += 1.0f;
        }

        if (keyboard_.key_down(sky::Key::up)) {
            cam_movement_.y -= 1.0f;
        }

        if (keyboard_.key_down(sky::Key::left)) {
            cam_movement_.x -= 1.0f;
        }

        if (keyboard_.key_down(sky::Key::right)) {
            cam_movement_.x += 1.0f;
        }

        cam_.move(cam_movement_ * cam_speed_);
        cam_mat_ = cam_.get_matrix();

        cmdqueue->update_uniform(viewproj_,
                                 sky::MemoryBlock { sizeof(sky::Matrix4f), &cam_mat_ });

        cmdqueue->draw();

        graphics_driver.commit_command_list();
    }

    void on_shutdown() override
    {

    }

private:
    sky::Path root_path_;
    sky::Keyboard keyboard_;
    sky::Font font_;

    uint32_t tex_, program_, vbuf_, viewproj_;

    std::vector<sky::Vertex> vertices_;

    sky::Camera2D cam_;
    float cam_speed_;
    sky::Vector2f cam_movement_;
    sky::Matrix4f cam_mat_;
};

int main(int argc, const char** argv)
{
    auto app = std::make_unique<TextApplication>();
    app->start(sky::GraphicsDriver::ThreadSupport::multi_threaded);
    return 0;
}

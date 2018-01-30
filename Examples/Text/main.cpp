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
#include <Skyrocket/Graphics/TextBuffer.hpp>

class TextApplication : public sky::Application {
public:
    TextApplication()
        : Application("Skyrocket Text Rendering Example"),
          tb_(&graphics_driver, &font_),
          cam_speed_(4.5f)
    {
        root_path_ = sky::Path::executable_path().relative_path("../../../../Examples/Text");
        if ( sky::target_platform == sky::OS::macos ) {
            root_path_ = sky::Path("/Users/Jacob/Dev/Repos/Skyrocket/Examples/Text");
        }
        root_path_.make_real();
    }

    void on_startup(int argc, const char** argv) override
    {
        primary_view.set_backing_color(sky::Color::cornflower_blue);
        cam_.setup(primary_view.size(), 0.01f, 1000.0f);
        cam_.set_position({0.0f, 0.0f});

        font_.load_from_file(root_path_.relative_path("Go-Regular.ttf"), 23);

        auto cmdqueue = graphics_driver.command_list();
        tb_.init();

        viewproj_ = cmdqueue->create_uniform(sky::UniformType::mat4, sizeof(sky::Matrix4f));

        auto vert_path = root_path_.relative_path("basic_vertex.metal");
        auto frag_path = root_path_.relative_path("basic_fragment.metal");
        program_ = cmdqueue->create_program(vert_path, frag_path);
        tb_.set_program(program_);
        tb_.set_text("Down", 4);
        graphics_driver.commit_command_list();
    }

    void on_update() override
    {
        cam_movement_.x = 0.0f;
        cam_movement_.y = 0.0f;

        if ( keyboard_.key_typed(sky::Key::escape) ) {
            primary_view.close();
        }
        if (keyboard_.key_down(sky::Key::down)) {
            cam_movement_.y += 1.0f;
            tb_.set_text("Down", 4);
        }
        if (keyboard_.key_down(sky::Key::up)) {
            cam_movement_.y -= 1.0f;
            tb_.set_text("Up", 2);
        }
        if (keyboard_.key_down(sky::Key::left)) {
            cam_movement_.x -= 1.0f;
            tb_.set_text("Left", 4);
        }
        if (keyboard_.key_down(sky::Key::right)) {
            cam_movement_.x += 1.0f;
            tb_.set_text("Right", 5);
        }

        cam_.move(cam_movement_ * cam_speed_);
        cam_mat_ = cam_.get_matrix();

        auto cmdqueue = graphics_driver.command_list();

        cmdqueue->set_state(sky::RenderPipelineState::culling_frontface);
        cmdqueue->update_uniform(viewproj_,
                                 sky::MemoryBlock { sizeof(sky::Matrix4f), &cam_mat_ });
        tb_.draw(viewproj_);

        graphics_driver.commit_command_list();
    }

    void on_shutdown() override
    {

    }

private:
    sky::Path root_path_;
    sky::Keyboard keyboard_;
    sky::Font font_;

    uint32_t program_, viewproj_;

    sky::TextBuffer tb_;

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

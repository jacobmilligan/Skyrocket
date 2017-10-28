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
        auto a = sky::Vertex(-1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        auto b = sky::Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f);
        auto c = sky::Vertex( 1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
        auto d = sky::Vertex( 1.0f,  1.0f,  1.0f, 1.0f, 0.1f, 0.6f, 0.4f, 1.0f, 0.0f, 1.0f);

        vertices_ = {a, b, c, a, c, d};

        primary_view.set_backing_color(sky::Color::cornflower_blue);

        font_.load_from_file(root_path_.relative_path("Go-Regular.ttf"), 23);
        tex_ = graphics_driver.create_texture(font_.width(), font_.height(),
                                              sky::PixelFormat::Enum::r8);
        sky::MemoryBlock mb = {
            static_cast<uint32_t>( sizeof(sky::Vertex) * vertices_.size() ),
            vertices_.data()
        };
        vbuf_ = graphics_driver.create_vertex_buffer(mb, sky::BufferUsage::staticbuf);
        graphics_driver.set_vertex_buffer(vbuf_, 0, vertices_.size());

        uint32_t x = 0;
        for ( auto& g : font_ ) {
            if ( g.size.x <= 0 || g.size.y <= 0 ) {
                continue;
            }
            graphics_driver.create_texture_region(tex_, sky::UIntRect(x, 0, g.size.x, g.size.y),
                                                  sky::PixelFormat::Enum::r8, g.data);
        }

        auto vert_path = root_path_.relative_path("basic_vertex.metal");
        auto frag_path = root_path_.relative_path("basic_fragment.metal");
        program_ = graphics_driver.create_program(vert_path, frag_path);
        graphics_driver.set_program(program_);
        graphics_driver.commit();
    }

    void on_update() override
    {
        if ( keyboard_.key_typed(sky::Key::escape) ) {
            primary_view.close();
        }

        graphics_driver.set_program(program_);
        graphics_driver.set_vertex_buffer(vbuf_, 0, vertices_.size());
        graphics_driver.set_texture(tex_, 0);
        graphics_driver.draw();
        graphics_driver.commit();
    }

    void on_shutdown() override
    {

    }

private:
    sky::Path root_path_;
    sky::Keyboard keyboard_;
    sky::Font font_;

    uint32_t tex_, program_, vbuf_;

    std::vector<sky::Vertex> vertices_;
};

int main(int argc, const char** argv)
{
    TextApplication app;
    app.start(sky::GraphicsDriver::ThreadSupport::single_thread);
}

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
        font_.load_from_file(root_path_.relative_path("Go-Regular.ttf"), 23);
    }

    void on_update() override
    {
        if ( keyboard_.key_typed(sky::Key::escape) ) {
            primary_view.close();
        }
    }

    void on_shutdown() override
    {

    }

private:
    sky::Path root_path_;
    sky::Keyboard keyboard_;
    sky::Font font_;

    uint32_t texture_;
};

int main(int argc, const char** argv)
{
    TextApplication app;
    app.start(sky::GraphicsDriver::ThreadSupport::single_thread);
}

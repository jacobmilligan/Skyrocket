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

class GraphicsApp : public sky::Application {
public:
    GraphicsApp()
        : sky::Application("Skyrocket Graphics Demo")
    {}

    void on_startup(int argc, const char** argv) override
    {
        window_id_ = platform_->create_window(app_name(), 800, 600);
    }

    void on_tick() override
    {
        if ( platform_->open_window_count() <= 0 )
            shutdown();
    }

    void on_key(sky::Key keycode) override
    {
        if ( keycode == sky::Key::escape ) {
            shutdown();
        }
    }

    void on_mouse() override
    {

    }

private:
    uint32_t window_id_;
};

int main(int argc, char** argv)
{
    GraphicsApp app;
    app.start();
    return 0;
}
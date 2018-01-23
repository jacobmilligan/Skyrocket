//
//  Playground.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 23/01/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include <Skyrocket/Framework/Application.hpp>
#include <Skyrocket/Graphics/GDI/Experimental/CommandBuffer.hpp>

class App : public sky::Application {
public:
    App()
        : sky::Application("Playground")
    {}

    void on_startup(int argc, const char** argv) override
    {
        sky::CommandBuffer buf{};

        buf.begin();
        buf.draw();
        buf.end();
    }

    void on_update() override
    {

    }

    void on_shutdown() override
    {

    }
};

int main(int argc, char** argv)
{
    App app;
    app.start(sky::GraphicsDriver::ThreadSupport::single_thread);
}
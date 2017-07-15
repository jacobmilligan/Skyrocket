//
//  GDI.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 15/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include <memory>

namespace sky {

class GDI {
public:
    GDI();

    ~GDI();

    void initialize();
private:
    struct Context;
    std::unique_ptr<Context> context_;
};


}
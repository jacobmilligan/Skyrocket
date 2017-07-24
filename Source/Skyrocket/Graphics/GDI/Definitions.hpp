//
//  Definitions.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 5/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

namespace sky {


enum class BufferUsage {
    none = 0x0,
    staticbuf = 0x1,
    dynamic
};

struct RenderCommand {
    enum class Type {
        set_shader
    };

    Type type;
    uint32_t id;
};


}  // namespace sky
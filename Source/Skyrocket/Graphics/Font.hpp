//
//  Font.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 11/08/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Core/Math/Vector2.hpp"
#include "Skyrocket/Platform/Filesystem.hpp"

namespace sky {


struct FontService;

struct Glyph {
    float offset;
    Vector2f size;
    Vector2f bearing;
    Vector2f advance;
    uint8_t* data;
};

struct Font {
    Font();
    ~Font();

    void load_from_file(const Path& path, const float pixel_size);
    void load_from_memory(uint8_t* memory, const float pixel_size);

    Glyph get_glyph(const char character);

    void set_pixel_size(const uint32_t size);
private:
    std::unique_ptr<FontService> service;
    std::vector<Glyph> glyphs_;

    void init_library();
    void reset_glyphs();
};


} // namespace sky
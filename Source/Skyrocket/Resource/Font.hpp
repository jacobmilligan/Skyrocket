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
#include "Skyrocket/Core/Geometry/Rectangle.hpp"
#include "Skyrocket/Platform/Filesystem.hpp"

namespace sky {


struct FontService;

struct Glyph {
    UIntRect bounds;
    uint8_t* data{};
    char character{};
    float s, t, s2, t2;
    Vector2<int32_t> bearing;
    Vector2<int32_t> advance;
};

struct Font {
    Font();
    ~Font();

    void load_from_file(const Path& path, const float pixel_size);
    void load_from_memory(uint8_t* memory, const float pixel_size);

    Glyph get_glyph(const char character);

    void set_pixel_size(const uint32_t size);

    inline uint32_t width()
    {
        return width_;
    }

    inline uint32_t height()
    {
        return height_;
    }

    inline uint32_t glyph_count()
    {
        return static_cast<uint32_t>(glyphs_.size());
    }

    inline uint32_t size()
    {
        return size_;
    }

    Glyph* begin()
    {
        return &glyphs_[0];
    }

    Glyph* end()
    {
        return &(*glyphs_.end());
    }
private:
    std::unique_ptr<FontService> service;
    std::vector<Glyph> glyphs_;

    uint32_t width_{0};
    uint32_t height_{0};
    uint32_t size_{0};

    void init_library();
    void reset_glyphs();
};


} // namespace sky
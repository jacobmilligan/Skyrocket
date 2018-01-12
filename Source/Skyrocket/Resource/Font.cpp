//
//  Font.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 11/08/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Core/Diagnostics/Error.hpp"
#include "Skyrocket/Resource/Font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

//===================================================
//  Implement error handler for freetype error table
//===================================================

#undef FTERRORS_H_
#define FT_ERRORDEF(e, v, s)  { e, s },
#define FT_ERROR_START_LIST     {
#define FT_ERROR_END_LIST       { 0, NULL } };

const struct {
    int code;
    const char* msg;
} FT_errors[] =

#include FT_ERRORS_H

//==========================================
//  End freetype error definitions
//==========================================

namespace sky {


struct FontService {
    ~FontService()
    {
        FT_Done_Face(face);
    }

    static FT_Library lib;
    FT_Face face;
};

FT_Library FontService::lib = nullptr;

Font::Font()
    : service(std::make_unique<FontService>())
{}

Font::~Font() = default;

void Font::init_library()
{
    if ( service->lib == nullptr ) {
        auto err = FT_Init_FreeType(&service->lib);
        if ( err == 1 ) {
            SKY_ERROR("FontService", "Unable to initialize freetype: %s", FT_errors[err].msg);
            return;
        }
    }
}

void Font::load_from_file(const Path& path, const float pixel_size)
{
    init_library();
    // This loads from file but we should allow loading from memory. The above
    // code is therefore redundant
    auto err = FT_New_Face(service->lib, path.str(), 0, &service->face);
    if ( err == 1 ) {
        SKY_ERROR("Font", "Unable to load font '%s': %s", path.str(), FT_errors[err].msg);
        return;
    }

    glyphs_.resize(static_cast<uint64_t>(service->face->num_glyphs));
    set_pixel_size(48);
}

void Font::load_from_memory(uint8_t* memory, const float pixel_size)
{
    SKY_ERROR("Font", "Load from memory not implemented");
}

Glyph Font::get_glyph(const char character)
{
    auto index = static_cast<uint32_t>(character);
    SKY_ASSERT(index < glyphs_.size(),
               "Font face doesn't contain a definition for '%c'", character);
    return glyphs_[index];
}

void Font::set_pixel_size(const uint32_t size)
{
    SKY_ASSERT(service->face != nullptr, "Font face has been loaded before assigning pixel size");

    auto err = FT_Set_Pixel_Sizes(service->face, 0, size);

    if ( err == 1) {
        SKY_ERROR("Font", "Unable to set font pixel size");
        return;
    }

    size_ = size;

    reset_glyphs();
}

void Font::reset_glyphs()
{
    width_ = 0;
    uint32_t c = 0;
    size_t bitmap_size = 0;
    FT_Error err;
    auto face = service->face;

    height_ = 0;
    uint32_t row = 0;
    uint32_t col = 0;
    uint32_t cur_row_height = 0;
    for ( auto& g : glyphs_ ) {
        auto index = FT_Get_Char_Index(face, c);

        err = FT_Load_Glyph(face, index, FT_LOAD_RENDER);
        if ( err == 1 ) {
            SKY_ERROR("Font", "Unable to load glyph for character %c: %s",
                      static_cast<char>(c), FT_errors[err].msg)
            continue;
        }

        err = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
        if ( err == 1 ) {
            SKY_ERROR("Font", "Unable to render glyph for character %c: %s",
                      static_cast<char>(c), FT_errors[err].msg)
            continue;
        }

        bitmap_size = face->glyph->bitmap.width * face->glyph->bitmap.rows;

        ++c;

        if ( index == 0 ) {
            g.bounds.width = 0;
            g.bounds.height = 0;
            g.data = nullptr;
            g.character = '\0';
            continue;
        }

        g.data = static_cast<uint8_t*>(malloc(bitmap_size));
        memcpy(g.data, face->glyph->bitmap.buffer, bitmap_size);

        g.character = static_cast<char>(c);

        g.bounds.width = face->glyph->bitmap.width;
        g.bounds.height = face->glyph->bitmap.rows;

        g.bearing.x = face->glyph->bitmap_left;
        g.bearing.y = face->glyph->bitmap_top;
        g.advance.x = static_cast<int32_t>(face->glyph->advance.x >> 6);
        g.advance.y = static_cast<int32_t>(face->glyph->advance.y >> 6);

        g.bounds.position.x = col;
        g.bounds.position.y = row;

        col += g.bounds.width + 1;

        if ( col > width_ ) {
            width_ = col;
        }

        if ( col >= 4096 ) {
            col = 0;
            row += cur_row_height;
            cur_row_height = 0;
        }

        cur_row_height = std::max(cur_row_height, g.bounds.height);
        height_ = std::max(height_, g.bounds.position.y + g.bounds.height);
    }

    for ( auto& g : glyphs_ ) {
        g.s = static_cast<float>(g.bounds.position.x) / static_cast<float>(width_);
        g.t = static_cast<float>(g.bounds.position.y) / static_cast<float>(height_);
        g.s2 = static_cast<float>(g.bounds.position.x + g.bounds.width) / static_cast<float>(width_);
        g.t2 = static_cast<float>(g.bounds.position.y + g.bounds.height) / static_cast<float>(height_);
    }
}


} // namespace sky
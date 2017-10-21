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
#include "Font.hpp"

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
    :
    service(std::make_unique<FontService>())
{}

Font::~Font() = default;

void Font::init_library()
{
    if ( service->lib == nullptr ) {
        auto err = FT_Init_FreeType(&service->lib);
        if ( err != 0 ) {
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
    if ( err != 0 ) {
        SKY_ERROR("Font", "Unable to load font '%s': %s", path.str(), FT_errors[err].msg);
        return;
    }

    glyphs_.resize(static_cast<uint64_t>(service->face->num_glyphs));
    reset_glyphs();
}

void Font::load_from_memory(uint8_t* memory, const float pixel_size)
{
    SKY_ERROR("Font", "Load from memory not implemented");
}

Glyph Font::get_glyph(const char character)
{
    auto index = static_cast<uint32_t>(character - 32);
    SKY_ASSERT(index > glyphs_.size(),
               "Font face doesn't contain a definition for '%c'",
               character);
    return glyphs_[index];
}

void Font::set_pixel_size(const uint32_t size)
{
    SKY_ASSERT(service->face != nullptr, "Font face has been loaded before assigning pixel size");

    auto err = FT_Set_Pixel_Sizes(service->face, size, size);

    if ( err != 0 ) {
        SKY_ERROR("Font", "Unable to set font pixel size");
        return;
    }

    reset_glyphs();
}

void Font::reset_glyphs()
{
    float offset = 0;
    uint32_t c = 32;
    FT_Error err;
    auto glyph = service->face->glyph;

    for ( auto& g : glyphs_ ) {
        err = FT_Load_Char(service->face, c, FT_LOAD_RENDER);
        if ( err != 0 ) {
            continue;
        }

        g.offset = offset;
        g.size.x = glyph->bitmap.width;
        g.size.y = glyph->bitmap.rows;
        g.bearing.x = glyph->bitmap_left;
        g.bearing.y = glyph->bitmap_top;
        g.advance.x = glyph->advance.x >> 6;
        g.advance.y = glyph->advance.y >> 6;
        g.data = glyph->bitmap.buffer;

        ++c;
        offset += g.size.x;
    }
}


} // namespace sky
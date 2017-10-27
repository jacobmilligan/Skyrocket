//
// Color.hpp
// Skyrocket Framework
//
// ----------------------------------------------------------------------------
//
// Created by Jacob Milligan on 8/12/2016.
// Copyright (c) 2016 Jacob Milligan All rights reserved.
//

#pragma once

#include <cstdint>

namespace sky {

/// @brief Structure representing a 4 component RGBA color value
struct Color {
    /// @brief Initializes a new color with the specified RGBA values
    /// @param cr The red component
    /// @param cg The green component
    /// @param cb The blue component
    /// @param ca The alpha component
    Color(const uint8_t cr, const uint8_t cg, const uint8_t cb,
          const uint8_t ca)
        :
        r(cr), g(cg), b(cb), a(ca)
    {}

    /// @brief Initializes a new color with the specified RGB values with the alpha
    /// value set to 255
    /// @param cr The red component
    /// @param cg The green component
    /// @param cb The blue component
    Color(const uint8_t cr, const uint8_t cg, const uint8_t cb)
        :
        r(cr), g(cg), b(cb), a(255)
    {}

    /// @brief Initializes a new color with all components set to 255
    Color()
        :
        r(255), g(255), b(255), a(255)
    {}

    /// @brief Copy constructor
    /// @param other The other color
    Color(const Color& other)
    {
        r = other.r;
        g = other.g;
        b = other.b;
        a = other.a;
    }

    bool operator==(const Color& other)
    {
        return other.r == r && other.g == g && other.b == b && other.a == a;
    }

    bool operator!=(const Color& other)
    {
        return other.r != r || other.g != g || other.b != b || other.a != a;
    }

    inline bool is_equal(const uint8_t& cr, const uint8_t& cg, const uint8_t& cb,
                         const uint8_t& ca) const
    {
        return cr == r && cg == g && cb == b && ca == a;
    }

    /// @brief The red component
    uint8_t r,
    /// @brief The green component
        g,
    /// @brief The blue component
        b,
    /// @brief The alpha component
        a;

    /// @brief Predefined black color
    static const Color black;
    /// @brief Predefined white color
    static const Color white;
    /// @brief Predefined red color
    static const Color red;
    /// @brief Predefined orange color
    static const Color orange;
    /// @brief Predefined green color
    static const Color green;
    /// @brief Predefined blue color
    static const Color blue;
    /// @brief Predefined cornflower blue color
    static const Color cornflower_blue;
    /// @brief Predefined purple color
    static const Color purple;
    /// @brief Predefined gray color
    static const Color gray;
    /// @brief Predefined transparent color
    static const Color transparent;
};

}

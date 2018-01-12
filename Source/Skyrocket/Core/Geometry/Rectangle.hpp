//
// Rectangle.hpp
// Skyrocket Framework
//
// ----------------------------------------------------------------------------
//
// Created by Jacob Milligan on 8/12/2016.
// Copyright (c) 2016 Jacob Milligan All rights reserved.
//

#pragma once

#include "Skyrocket/Core/Math/Vector2.hpp"
#include "Skyrocket/Graphics/Color.hpp"

namespace sky {

/// @brief Rectangle is a 2D rectangle type with an x, y, width, height and several
/// helpful utility functions defined for it.
template<typename T>
struct Rectangle {

    /// @brief Initializes a new, zero-sized Rectangle at position {0,0}
    Rectangle()
        :
        position(0, 0), width(0), height(0)
    {}

    Rectangle(const Vector2<T> pos, const Vector2<T> size)
        :
        position(pos), width(size.x), height(size.y)
    {}

    /// @brief Initializes a new Rectangle with the specified position, width, and
    /// height
    /// @param rpos The rectangles world-based position
    /// @param rwidth The rectangles width
    /// @param rheight The rectangles height
    Rectangle(const Vector2<T>& rect_pos, const T rect_width, const T rect_height)
        :
        position(rect_pos), width(rect_width), height(rect_height)
    {}

    /// @brief Initializes a new Rectangle with the specified x and y position with
    /// the specified width and height
    /// @param x The rectangles x coordinate
    /// @param y The rectangles y coordinate
    /// @param rwidth The rectangles width
    /// @param rheight The rectangles height
    Rectangle(const T x, const T y, const T rect_width, const T rect_height)
        :
        position(x, y), width(rect_width), height(rect_height)
    {}

    /// @brief The rectangles world-based position in pixels
    Vector2<T> position;
    /// @brief The rectangles width in pixels
    T width;
    /// @brief The rectangles height in pixels
    T height;

    /// @brief Gets the Y coordinate of the Rectangles top-most edge
    /// @return The top coordinate
    T top() const
    {
        return position.y;
    }

    /// @brief Gets the X coordinate of the Rectangles right-most edge
    /// @return The right coordinate
    T right() const
    {
        return position.x + width;
    }

    /// @brief Gets the Y coordinate of the Rectangles bottom-most edge
    /// @return The bottom coordinate
    T bottom() const
    {
        return position.y + height;
    }

    /// @brief Gets the X coordinate of the Rectangles left-most edge
    /// @return The left coordinate
    T left() const
    {
        return position.x;
    }

    /// @brief Gets a vector representing the Rectangles center point
    /// @return The rectangles center point
    Vector2f center() const
    {
        auto x = position.x + (0.5f * width);
        auto y = position.y + (0.5f * height);
        return Vector2f(x, y);
    }

    /// @brief Checks to see if a particular Rectangle is contained within this
    /// Rectangles boundaries without overflow
    /// @param rect The rectangle to check
    /// @return True if the rectangle is contained within, false otherwise
    bool contains(const Rectangle<T>& rect) const
    {
        return (position.x < rect.position.x + rect.width)
            && (position.x + width > rect.position.y)
            && (position.y < rect.position.y + rect.height)
            && (position.y + height > rect.position.y);
    }

    /// @brief Checks to see if a particular point in world-space is contained
    /// within this Rectangles boundaries
    /// @param rect The point vector to check
    /// @return True if the point is contained within, false otherwise
    bool contains(const Vector2f& point) const
    {
        return (point.x >= position.x && point.x < position.x + width)
            && (point.y >= position.y && point.y < position.y + height);
    }

    /// @brief Checks to see if a particular Rectangle intersects with this
    /// rectangle. An intersection is defined if any single point is found within
    /// or on the boundaries of either rectangle.
    /// @param rect The rectangle to check
    /// @return True is the rectangle intersects this one, false otherwise
    bool intersects(const Rectangle<T>& rect) const
    {
        return position.x < rect.position.x + rect.width
            && position.x + width > rect.position.x
            && position.y < rect.position.y + rect.height
            && position.y + height > rect.position.y;
    }

    bool operator==(const Rectangle<T>& other) const
    {
        return other.position == position
            && other.width == width
            && other.height == height;
    }

    bool operator!=(const Rectangle<T>& other) const
    {
        return other.position != position
            || other.width != width
            || other.height != height;
    }

};


using IntRect = Rectangle<int32_t>;
using UIntRect = Rectangle<uint32_t>;
using FloatRect = Rectangle<float>;
using DoubleRect = Rectangle<double>;

}  // namespace sky

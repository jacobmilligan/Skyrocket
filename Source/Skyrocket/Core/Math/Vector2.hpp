//
// Vector.hpp
// Skyrocket Framework
//
// ----------------------------------------------------------------------------
//
// Created by Jacob Milligan on 8/12/2016.
// Copyright (c) 2016 Jacob Milligan All rights reserved.
//

#pragma once

#include "Skyrocket/Core/Math/Vector2.hpp"
#include "Skyrocket/Core/Math/Common.hpp"
#include "Skyrocket/Core/Diagnostics/Error.hpp"

#include <string>
#include <sstream>
#include <cmath>
#include <cstdint>

namespace sky {

/// @ingroup linear
/// @{

/// @brief A 2 dimensional Vector structure with several linear algebra operations
/// defined for its members.
/// @tparam T The data type to store in the vector
template <typename T>
struct Vector2 {
    
    union {
        struct {
            /// @brief The x coordinate of the Vector2
            T x;
            
            /// @brief  The y coordinate of the Vector2
            T y;
        };
    };
    
    /// @brief Initializes a new Vector2 with all elements assigned the
    /// specified value
    /// @param value Value to assign to all elements
    Vector2(const T value)
        : x(value), y(value) {}
    
    /// @brief Initializes a new Vector2 with all elements assigned a value of zero
    Vector2() : Vector2(0) {}
    
    /// @brief Initializes a new Vector2 with each element assigned the
    /// specified value
    /// @param vx Value to assign to the x element (element[0])
    /// @param vy Value to assign to the y element (element[1])
    Vector2(const T vx, const T vy)
        : x(vx), y(vy) {}

    /// @brief Outputs a string representation of the Vector2 elements in the form
    /// "{x, y}"
    /// @return The string representation of the Vector2
    std::string to_string()
    {
        std::stringstream ss;
        ss << "{" << x << ", " << y << "}";
        return ss.str();
    }
    
    /// @brief Computes the dot product of this Vector2 and another, returning
    /// a value of type T
    /// @param right The vector to compute the dot product with
    /// @return Dot product result
    T dot(const Vector2<T> &right) const
    {
        return (x * right.x) + (y * right.y);
    }
    
    /// @brief Computes the length of the Vector2
    /// @return The vectors length
    T length() const
    {
        return std::sqrt( (x * x) + (y * y) );
    }

    /// @brief Computes the squared length of the Vector2 - used for optimizing
    /// simple length comparisons when exact length value isn't required
    /// @return The vectors length squared
    T squared_length() const
    {
        return (x * x) + (y * y);
    }
    
    /// @brief Normalizes the Vector2's elements to be of length 1 but retain their
    /// direction.
    void normalize()
    {
        auto len = length();
        auto n = static_cast<T>(1) / ( (len <= 0) ? 1 : len );
        
        x *= n;
        y *= n;
    }

    /// @brief Normalizes the Vector2's elements to be of length 1 but retain their
    /// direction.
    /// @return The normalized version of the vector
    Vector2<T> get_normalized()
    {
        auto normalized = *this;
        auto len = length();
        auto n = static_cast<T>(1) / ( (len <= 0) ? 1 : len );

        return Vector2<T>(x * n, y * n);
    }

    Vector2<T> perpendicular() const
    {
        return Vector2<T>(-y, x);
    }
    
    /// @brief Clamps all of the Vector2's elements to remain within the given
    /// upper and lower bounds.
    /// @param lower Vector2 representing the lowest allowed values
    /// @param upper Vector2 representing the highest allowed values
    void clamp(const Vector2<T>& lower, const Vector2<T>& upper)
    {
        x = math::clamp<T>(x, lower.x, upper.x);
        y = math::clamp<T>(y, lower.y, upper.y);
    }

    /// @brief Clamps all of the Vector2's elements to remain within the given
    /// upper and lower bounds.
    /// @param lower T value representing the lowest allowed value applied to each component
    /// @param upper T value representing the highest allowed value applied to each component
    void clamp(const T& lower, const T& upper)
    {
        x = math::clamp<T>(x, lower, upper);
        y = math::clamp<T>(y, lower, upper);
    }

    float distance(const Vector2<T>& target) const
    {
        auto ydist = y - target.y;
        auto xdist = x - target.x;
        return static_cast<float>(sqrt( (ydist * ydist) + (xdist * xdist) ));
    }

    // Arithmetic operators
    
    /// @brief Unary operator += overload for Vector2 value
    /// @param right Vector2 to add
    /// @return The Vector2 result
    Vector2<T>& operator+=(const Vector2<T>& right)
    {
        x += right.x;
        y += right.y;
        return *this;
    }
    
    /// @brief Unary operator += overload for scalar value
    /// @param scalar Scalar to add to all elements
    /// @return The Vector2 result
    Vector2<T>& operator+=(const T& scalar)
    {
        x += scalar;
        y += scalar;
        return *this;
    }
    
    /// @brief Unary operator -= overload for Vector2 value
    /// @param right Vector2 to subtract
    /// @return The Vector2 result
    Vector2<T>& operator-=(const Vector2<T>& right)
    {
        x -= right.x;
        y -= right.y;
        return *this;
    }
    
    /// @brief Unary operator -= overload for scalar value
    /// @param scalar Scalar value to subtract from all elements
    /// @return The Vector2 result
    Vector2<T>& operator-=(const T& scalar)
    {
        x -= scalar;
        y -= scalar;
        return *this;
    }
    
    /// @brief Unary operator *= overload for Vector2 value
    /// @param right Vector2 to multiply
    /// @return The Vector2 result
    Vector2<T>& operator*=(const Vector2<T>& right)
    {
        x *= right.x;
        y *= right.y;
        return *this;
    }
    
    /// @brief Unary operator *= overload for scalar value
    /// @param scalar Scalar value to multiply with all elements
    /// @return The Vector2 result
    Vector2<T>& operator*=(const T& scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    
    /// @brief Unary operator /= overload for Vector2 value
    /// @param right Vector2 to divide
    /// @return The Vector2 result
    Vector2<T>& operator/=(const Vector2<T>& right)
    {
        x /= right.x;
        y /= right.y;
        return *this;
    }
    
    /// @brief Binary operator /= overload for scalar value
    /// @param scalar Scalar value to divide all elements by
    /// @return The Vector2 result
    Vector2<T>& operator/=(const T& scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }
    
    /// @brief Operator[] overload for element access
    /// @param i The element index to access
    /// @return The element at index i
    T& operator[](const int i)
    {
        SKY_ASSERT(i < 2, "Index %d out of bounds", i);
        
        return (&x)[i];
    }
    
    /// @brief Operator[] overload for const element access
    /// @param i The element index to access
    /// @return The element at index i
    T const& operator[](const int i) const
    {
        SKY_ASSERT(i < 2, "Index %d out of bounds", i);
        
        return (&x)[i];
    }
};

//////////////////////////
// Non-Member overloads
//////////////////////////

// Arithmetic operators

/// @brief Binary operator+ overload with a Vector value
/// @tparam T The Vector2's data type
/// @param left Left operand
/// @param right Right operand
/// @return Result of addition of each of the lefts elements with the rights
template <typename T>
Vector2<T> operator+(const Vector2<T>& left, const Vector2<T>& right)
{
    return Vector2<T>(
        left.x + right.x,
        left.y + right.y
    );
}

/// @brief Binary operator+ overload with a scalar value
/// @tparam T The Vector2 and scalars data type
/// @param vec Left operand
/// @param scalar Right operand (scalar)
/// @return Addition result of each of the vectors elements with the scalar.
template <typename T>
Vector2<T> operator+(const Vector2<T>& vec, const T& scalar)
{
    return Vector2<T>(
        vec.x + scalar,
        vec.y + scalar
    );
}

/// @brief Binary operator- overload
/// @tparam T Data type of each Vector2
/// @param left Left operand
/// @param right Right operand
/// @return Subtraction result of each of the lefts elements by the right elements
template <typename T>
Vector2<T> operator-(const Vector2<T>& left, const Vector2<T>& right)
{
    return Vector2<T>(
        left.x - right.x,
        left.y - right.y
    );
}

/// @brief Binary operator- overload with a scalar value
/// @tparam T Data type of the Vector2 and the scalar
/// @param left Left operand
/// @param right Right operand (scalar)
/// @return Subtraction result of each of the vectors elements by the scalar
template <typename T>
Vector2<T> operator-(const Vector2<T>& vec, const T& scalar)
{
    return Vector2<T>(
        vec.x - scalar,
        vec.y - scalar
    );
}

/// @brief Binary operator/ overload
/// @tparam T Data type of each Vector2
/// @param left Left operand
/// @param right Right operand
/// @return Division result of each of the left operands elements over the right elements
template <typename T>
Vector2<T> operator/(const Vector2<T>& left, const Vector2<T>& right)
{
    return Vector2<T>(
        left.x / right.x,
        left.y / right.y
    );
}

/// @brief Binary operator/ overload with a scalar value
/// @tparam T Data type of the Vector2 and the scalar
/// @param left Left operand
/// @param right Right operand
/// @return Division result of each of the left operands elements over the scalar
template <typename T>
Vector2<T> operator/(const Vector2<T>& vec, const T& divisor)
{
    return Vector2<T>(
        vec.x / divisor,
        vec.y / divisor
    );
}

/// @brief Binary operator* overload
/// @tparam T Data type of the Vector2's
/// @param left The left operand
/// @param right The right operand
/// @return Multiplication result of each of the vectors elements
template <typename T>
Vector2<T> operator*(const Vector2<T>& left, const Vector2<T>& right)
{
    return Vector2<T>(
        left.x * right.x,
        left.y * right.y
    );
}

/// @brief Binary operator* overload with a scalar value
/// @tparam T Data type of the Vector2 and the scalar
/// @param left The left operand
/// @param right The right operand
/// @return Multiplication result of each of the vectors elements with the scalar
template <typename T>
Vector2<T> operator*(const Vector2<T>& vec, const T& scalar)
{
    return Vector2<T>(
        vec.x * scalar,
        vec.y * scalar
    );
}

/// @brief Unary operator- overload (negation)
/// @tparam T Data type of the Vector2's elements
/// @param vec The vector to negate
/// @return The result of negating each of the vectors elements
template <typename T>
Vector2<T> operator-(const Vector2<T>& vec)
{
    return Vector2<T>(
        -vec.x,
        -vec.y
    );
}

// Logical operators

/// @brief Binary operator== overload
/// @tparam T Data type of each Vector2
/// @param left Left operand
/// @param right Right operand
/// @return true if all elements in both Vector2's are equal, false otherwise
template <typename T>
bool operator==(const Vector2<T>& left, const Vector2<T> &right)
{
    return (left.x == right.x) && (left.y == right.y);
}

/// @brief Binary operator!= overload
/// @tparam T Data type of each Vector2
/// @param left Left operand
/// @param right Right operand
/// @return true if any element in either Vector2 are not equal, false otherwise
template <typename T>
bool operator!=(const Vector2<T>& left, const Vector2<T> &right)
{
    return (left.x != right.x) || (left.y != right.y);
}

/// @brief Type alias for a Vector2 with elements of type double
using Vector2d = Vector2<double>;
/// @brief Type alias for a Vector2 with elements of type float
using Vector2f = Vector2<float>;
/// @brief Type alias for a Vector2 with elements of type 32 bit int
using Vector2i = Vector2<int32_t>;
/// @brief Type alias for a Vector2 with elements of type 32 bit unsigned int
using Vector2u = Vector2<uint32_t>;
/// @brief Type alias for a Vector2 with elements of type bool
using Vector2b = Vector2<bool>;

/// @}

}

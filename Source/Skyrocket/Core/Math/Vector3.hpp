//
//  Vector3.hpp
//  Skyrocket Framework
//
//  --------------------------------------------------------------
//
//  Created by Jacob Milligan on 10/12/2016.
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Vector2.hpp"

#include <string>

namespace sky {


/// @brief A 3 dimensional Vector structure with several linear algebra operations
/// defined for its members.
/// @tparam T The data type to store in the vector
template<typename T>
struct Vector3 {

    union {
        struct { T x, y, z; };
        struct { T r, g, b; };
        struct { T s, t, p; };
    };

    /// @brief Initializes a new Vector3 with all elements assigned the
    /// specified value
    /// @param value Value to assign to all elements
    explicit Vector3(const T value)
        :
        x(value), y(value), z(value)
    {}

    /// @brief Initializes a new Vector3 with all elements assigned a value of zero
    Vector3() :
        Vector3(0.0f)
    {}

    /// @brief Initializes a new Vector3 with each element assigned the
    /// specified value
    /// @param vx Value to assign to the x element (element[0])
    /// @param vy Value to assign to the y element (element[1])
    /// @param vz Value to assign to the z element (element[2])
    Vector3(const T vx, const T vy, const T vz)
        :
        x(vx), y(vy), z(vz)
    {}

    /// @brief Initializes a new Vector3 using a Vector2 for the x and y
    /// coordinates
    /// @param vec2 Vector2 to use for x and y coordinates
    /// @param vz The z coordinate value
    Vector3(const Vector2<T>& vec2, const T vz)
        :
        x(vec2.x), y(vec2.y), z(vz)
    {}

    /// @brief Outputs a string representation of the Vector3 elements in the form
    /// "{x, y, z}"
    /// @return The string representation of the Vector3
    std::string to_string()
    {
        std::stringstream ss;
        ss << "{" << x << ", " << y << ", " << z << "}";
        return ss.str();
    }

    /// @brief Computes the dot product of this Vector3 and another, returning
    /// a value of type T
    /// @param right The vector to compute the dot product with
    /// @return Dot product result
    T dot(const Vector3<T>& right) const
    {
        return (x * right.x) + (y * right.y) + (z * right.z);
    }

    /// @brief Computes the length of the Vector3
    /// @return The vectors length
    T length() const
    {
        return std::sqrt((x * x) + (y * y) + (z * z));
    }

    /// @brief Computes the squared length of the Vector3 - used for optimizing
    /// simple length comparisons when exact length value isn't required
    /// @return The vectors length squared
    T squared_length() const
    {
        return (x * x) + (y * y) + (z * z);
    }

    /// @brief Normalizes the Vector3's elements to be of length 1 but retain their
    /// direction.
    void normalize()
    {
        auto len = length();
        auto n = static_cast<T>(1) / ((len <= 0) ? 1 : len);

        x *= n;
        y *= n;
        z *= n;
    }

    /// @brief Normalizes the Vector3's elements to be of length 1 but retain their
    /// direction.
    /// @return The normalized version of the vector
    Vector3<T> get_normalized() const
    {
        auto len = length();
        auto n = static_cast<T>(1) / ((len <= 0) ? 1 : len);

        return Vector3<T>(x * n, y * n, z * n);
    }

    /// @brief Clamps all of the Vector3's elements to remain within the given
    /// upper and lower bounds.
    /// @param lower Vector3 representing the lowest allowed values
    /// @param upper Vector3 representing the highest allowed values
    void clamp(const Vector3<T>& lower, const Vector3<T>& upper)
    {
        x = math::clamp<T>(x, lower.x, upper.x);
        y = math::clamp<T>(y, lower.y, upper.y);
        z = math::clamp<T>(z, lower.z, upper.z);
    }

    /// @brief Clamps all of the Vector2's elements to remain within the given
    /// upper and lower bounds.
    /// @param lower T value representing the lowest allowed value applied to each component
    /// @param upper T value representing the highest allowed value applied to each component
    void clamp(const T& lower, const T& upper)
    {
        x = math::clamp<T>(x, lower, upper);
        y = math::clamp<T>(y, lower, upper);
        z = math::clamp<T>(z, lower, upper);
    }

    /// @brief Gets the distance from this vector to a target vector
    /// @param target
    /// @return
    float distance(const Vector3<T>& target) const
    {
        auto ydist = y - target.y;
        auto xdist = x - target.x;
        auto zdist = z - target.z;
        return static_cast<float>(sqrt(
            (ydist * ydist) + (xdist * xdist) + (zdist * zdist)
        ));
    }

    /// @brief Calculates the vectors cross product with the given Vector3 to
    /// produce a new Vector3 that is perpendicular to to both operands
    /// @param right Right operand
    /// @return Vector3 that is perpendicular to both operands
    Vector3<T> cross(const Vector3<T>& right) const
    {
        return Vector3<T>(
            (y * right.z) - (z * right.y),
            (z * right.x) - (x * right.z),
            (x * right.y) - (y * right.x)
        );
    }

    // Arithmetic operators

    /// @brief Unary operator += overload for Vector3 value
    /// @param right Vector3 to add
    /// @return The Vector3 result
    Vector3<T>& operator+=(const Vector3<T>& right)
    {
        x += right.x;
        y += right.y;
        z += right.z;
        return *this;
    }

    /// @brief Unary operator += overload for scalar value
    /// @param scalar Scalar to add to all elements
    /// @return The Vector3 result
    Vector3<T>& operator+=(const T& scalar)
    {
        x += scalar;
        y += scalar;
        z += scalar;
        return *this;
    }

    /// @brief Unary operator -= overload for Vector3 value
    /// @param right Vector3 to subtract
    /// @return The Vector3 result
    Vector3<T>& operator-=(const Vector3<T>& right)
    {
        x -= right.x;
        y -= right.y;
        z -= right.z;
        return *this;
    }

    /// @brief Unary operator -= overload for scalar value
    /// @param scalar Scalar value to subtract from all elements
    /// @return The Vector3 result
    Vector3<T>& operator-=(const T& scalar)
    {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        return *this;
    }

    /// @brief Unary operator *= overload for Vector3 value
    /// @param right Vector3 to multiply
    /// @return The Vector3 result
    Vector3<T>& operator*=(const Vector3<T>& right)
    {
        x *= right.x;
        y *= right.y;
        z *= right.z;
        return *this;
    }

    /// @brief Unary operator *= overload for scalar value
    /// @param scalar Scalar value to multiply with all elements
    /// @return The Vector3 result
    Vector3<T>& operator*=(const T& scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    /// @brief Unary operator /= overload for Vector3 value
    /// @param right Vector3 to divide
    /// @return The Vector3 result
    Vector3<T>& operator/=(const Vector3<T>& right)
    {
        x /= right.x;
        y /= right.y;
        z /= right.z;
        return *this;
    }

    /// @brief Binary operator /= overload for scalar value
    /// @param scalar Scalar value to divide all elements by
    /// @return The Vector3 result
    Vector3<T>& operator/=(const T& scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    /// @brief Operator[] overload for element access
    /// @param i The element index to access
    /// @return The element at index i
    T& operator[](const int i)
    {
        SKY_ASSERT(i < 3, "Index out of bounds");

        return (&x)[i];
    }

    /// @brief Operator[] overload for const element access
    /// @param i The element index to access
    /// @return The element at index i
    T const& operator[](const int i) const
    {
        SKY_ASSERT(i < 3, "Index out of bounds");

        return (&x)[i];
    }
};

//////////////////////////
// Non-Member overloads
//////////////////////////

// Arithmetic operators

/// @brief Binary operator+ overload with a Vector value
/// @tparam T The Vector3's data type
/// @param left Left operand
/// @param right Right operand
/// @return Result of addition of each of the lefts elements with the rights
template<typename T>
Vector3<T> operator+(const Vector3<T>& left, const Vector3<T>& right)
{
    return Vector3<T>(
        left.x + right.x,
        left.y + right.y,
        left.z + right.z
    );
}

/// @brief Binary operator+ overload with a scalar value
/// @tparam T The Vector3 and scalars data type
/// @param vec Left operand
/// @param scalar Right operand (scalar)
/// @return Addition result of each of the vectors elements with the scalar.
template<typename T>
Vector3<T> operator+(const Vector3<T>& vec, const T& scalar)
{
    return Vector3<T>(
        vec.x + scalar,
        vec.y + scalar,
        vec.z + scalar
    );
}

/// @brief Binary operator- overload
/// @tparam T Data type of each Vector3
/// @param left Left operand
/// @param right Right operand
/// @return Subtraction result of each of the lefts elements by the right elements
template<typename T>
Vector3<T> operator-(const Vector3<T>& left, const Vector3<T>& right)
{
    return Vector3<T>(
        left.x - right.x,
        left.y - right.y,
        left.z - right.z
    );
}

/// @brief Binary operator- overload with a scalar value
/// @tparam T Data type of the Vector3 and the scalar
/// @param left Left operand
/// @param right Right operand (scalar)
/// @return Subtraction result of each of the vectors elements by the scalar
template<typename T>
Vector3<T> operator-(const Vector3<T>& vec, const T& scalar)
{
    return Vector3<T>(
        vec.x - scalar,
        vec.y - scalar,
        vec.z - scalar
    );
}

/// @brief Binary operator/ overload
/// @tparam T Data type of each Vector3
/// @param left Left operand
/// @param right Right operand
/// @return Division result of each of the left operands elements over the right elements
template<typename T>
Vector3<T> operator/(const Vector3<T>& left, const Vector3<T>& right)
{
    return Vector3<T>(
        left.x / right.x,
        left.y / right.y,
        left.z / right.z
    );
}

/// @brief Binary operator/ overload with a scalar value
/// @tparam T Data type of the Vector3 and the scalar
/// @param left Left operand
/// @param right Right operand
/// @return Division result of each of the left operands elements over the scalar
template<typename T>
Vector3<T> operator/(const Vector3<T>& vec, const T& divisor)
{
    return Vector3<T>(
        vec.x / divisor,
        vec.y / divisor,
        vec.z / divisor
    );
}

/// @brief Binary operator* overload
/// @tparam T Data type of the Vector3's
/// @param left The left operand
/// @param right The right operand
/// @return Multiplication result of each of the vectors elements
template<typename T>
Vector3<T> operator*(const Vector3<T>& left, const Vector3<T>& right)
{
    return Vector3<T>(
        left.x * right.x,
        left.y * right.y,
        left.z * right.z
    );
}

/// @brief Binary operator* overload with a scalar value
/// @tparam T Data type of the Vector3 and the scalar
/// @param left The left operand
/// @param right The right operand
/// @return Multiplication result of each of the vectors elements with the scalar
template<typename T>
Vector3<T> operator*(const Vector3<T>& vec, const T& scalar)
{
    return Vector3<T>(
        vec.x * scalar,
        vec.y * scalar,
        vec.z * scalar
    );
}

/// @brief Unary operator- overload (negation)
/// @tparam T Data type of the Vector3's elements
/// @param vec The vector to negate
/// @return The result of negating each of the vectors elements
template<typename T>
Vector3<T> operator-(const Vector3<T>& vec)
{
    return Vector3<T>(
        -vec.x,
        -vec.y,
        -vec.z
    );
}

// Logical operators

/// @brief Binary operator== overload
/// @tparam T Data type of each Vector3
/// @param left Left operand
/// @param right Right operand
/// @return true if all elements in both Vector3's are equal, false otherwise
template<typename T>
bool operator==(const Vector3<T>& left, const Vector3<T>& right)
{
    return (left.x == right.x) && (left.y == right.y) && (left.z == right.z);
}

/// @brief Binary operator!= overload
/// @tparam T Data type of each Vector3
/// @param left Left operand
/// @param right Right operand
/// @return true if any element in either Vector3 are not equal, false otherwise
template<typename T>
bool operator!=(const Vector3<T>& left, const Vector3<T>& right)
{
    return (left.x != right.x) || (left.y != right.y) || (left.z != right.z);;
}

/// @defgroup vec3aliases Vector3 Aliases
/// @{

/// @brief Type alias for a Vector3 with elements of type double
using Vector3d = Vector3<double>;
/// @brief Type alias for a Vector3 with elements of type float
using Vector3f = Vector3<float>;
/// @brief Type alias for a Vector3 with elements of type 32 bit int
using Vector3i = Vector3<int32_t>;
/// @brief Type alias for a Vector3 with elements of type 32 bit unsigned int
using Vector3u = Vector3<uint32_t>;
/// @brief Type alias for a Vector3 with elements of type bool
using Vector3b = Vector3<bool>;

/// @}

}  // namespace sky

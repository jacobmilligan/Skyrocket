//
//  Vector4.hpp
//  Skyrocket Framework
//
//  --------------------------------------------------------------
//
//  Created by Jacob Milligan on 10/12/2016.
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Vector3.hpp"

#include <string>

namespace sky {


/// @brief A 4 dimensional Vector structure with several linear algebra operations
/// defined for its members.
/// @tparam T The data type to store in the vector
template<typename T>
struct Vector4 {

    union {
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
        struct { T s, t, p, q; };
    };

    /// @brief Initializes a new Vector4 with all elements assigned the
    /// specified value
    /// @param value Value to assign to all elements
    SKY_FORCE_INLINE explicit Vector4(const T value)
        : x(value), y(value), z(value), w(value)
    {}

    /// @brief Initializes a new Vector4 with all elements assigned a value of zero
    SKY_FORCE_INLINE Vector4()
        : Vector4(0.0f)
    {}

    /// @brief Initializes a new Vector4 with each element assigned the
    /// specified value
    /// @param vx Value to assign to the x element (element[0])
    /// @param vy Value to assign to the y element (element[1])
    /// @param vz Value to assign to the z element (element[2])
    /// @param vw Value to assign to the w element (element[3])
    SKY_FORCE_INLINE Vector4(const T vx, const T vy, const T vz, const T vw)
        :
        x(vx), y(vy), z(vz), w(vw)
    {}

    /// @brief Initializes a new Vector4 using a Vector2 for the x and y
    /// coordinates
    /// @param vec2 Vector2 to use for x and y coordinates
    /// @param vz The z coordinate value
    /// @param vw The w coordinate value
    SKY_FORCE_INLINE Vector4(const Vector2<T>& vec2, const T vz, const T vw)
        :
        x(vec2.x), y(vec2.y), z(vz), w(vw)
    {}

    /// @brief Initializes a new Vector4 using a Vector3 for the x, y, and z
    /// coordinates
    /// @param vec2 Vector3 to use for x, y, and z coordinates
    /// @param vw The w coordinate value
    SKY_FORCE_INLINE Vector4(const Vector3<T>& vec3, const T vw)
        :
        x(vec3.x), y(vec3.y), z(vec3.z), w(vw)
    {}

    /// @brief Outputs a string representation of the Vector4 elements in the form
    /// "{x, y, z, w}"
    /// @return The string representation of the Vector4
    std::string to_string()
    {
        std::stringstream ss;
        ss << "{" << x << ", " << y << ", " << z << ", " << w << "}";
        return ss.str();
    }

    /// @brief Computes the dot product of this Vector4 and another, returning
    /// a value of type T
    /// @param right The vector to compute the dot product with
    /// @return Dot product result
    SKY_FORCE_INLINE T dot(const Vector4<T>& right) const
    {
        return (x * right.x) + (y * right.y) + (z * right.z) + (w * right.w);
    }

    /// @brief Computes the length of the Vector4
    /// @return The vectors length
    SKY_FORCE_INLINE T length() const
    {
        return std::sqrt((x * x) + (y * y) + (z * z) + (w * w));
    }

    /// @brief Computes the squared length of the Vector4 - used for optimizing
    /// simple length comparisons when exact length value isn't required
    /// @return The vectors length squared
    SKY_FORCE_INLINE T squared_length() const
    {
        return (x * x) + (y * y) + (z * z) + (w * w);
    }

    /// @brief Normalizes the Vector4's elements to be of length 1 but retain their
    /// direction.
    SKY_FORCE_INLINE void normalize()
    {
        auto len = length();
        auto n = static_cast<T>(1) / ((len <= 0) ? 1 : len);

        x *= n;
        y *= n;
        z *= n;
        w *= n;
    }

    /// @brief Normalizes the Vector4's elements to be of length 1 but retain their
    /// direction.
    /// @return The normalized version of the vector
    SKY_FORCE_INLINE Vector4<T> get_normalized() const
    {
        auto normalized = *this;
        auto len = length();
        auto n = static_cast<T>(1) / ((len <= 0) ? 1 : len);

        return Vector4<T>(x * n, y * n, z * n, w * n);
    }

    /// @brief Clamps all of the Vector4's elements to remain within the given
    /// upper and lower bounds.
    /// @param lower Vector4 representing the lowest allowed values
    /// @param upper Vector4 representing the highest allowed values
    SKY_FORCE_INLINE void clamp(const Vector4<T>& lower, const Vector4<T>& upper)
    {
        x = math::clamp<T>(x, lower.x, upper.x);
        y = math::clamp<T>(y, lower.y, upper.y);
        z = math::clamp<T>(z, lower.z, upper.z);
        w = math::clamp<T>(w, lower.w, upper.w);
    }

    /// @brief Clamps all of the Vector2's elements to remain within the given
    /// upper and lower bounds.
    /// @param lower T value representing the lowest allowed value applied to each component
    /// @param upper T value representing the highest allowed value applied to each component
    SKY_FORCE_INLINE void clamp(const T& lower, const T& upper)
    {
        x = math::clamp<T>(x, lower, upper);
        y = math::clamp<T>(y, lower, upper);
        z = math::clamp<T>(z, lower, upper);
        w = math::clamp<T>(w, lower, upper);
    }

    /// @brief Gets the distance from this vector to a target vector
    /// @param target
    /// @return
    SKY_FORCE_INLINE float distance(const Vector4<T>& target) const
    {
        auto ydist = y - target.y;
        auto xdist = x - target.x;
        auto zdist = z - target.z;
        auto wdist = w - target.w;
        return static_cast<float>(sqrt(
            (ydist * ydist) + (xdist * xdist) + (zdist * zdist) + (wdist * wdist)
        ));
    }

    // Arithmetic operators

    /// @brief Unary operator += overload for Vector4 value
    /// @param right Vector4 to add
    /// @return The Vector4 result
    SKY_FORCE_INLINE Vector4<T>& operator+=(const Vector4<T>& right)
    {
        x += right.x;
        y += right.y;
        z += right.z;
        w += right.w;
        return *this;
    }

    /// @brief Unary operator += overload for scalar value
    /// @param scalar Scalar to add to all elements
    /// @return The Vector4 result
    SKY_FORCE_INLINE Vector4<T>& operator+=(const T& scalar)
    {
        x += scalar;
        y += scalar;
        z += scalar;
        w += scalar;
        return *this;
    }

    /// @brief Unary operator -= overload for Vector4 value
    /// @param right Vector4 to subtract
    /// @return The Vector4 result
    SKY_FORCE_INLINE Vector4<T>& operator-=(const Vector4<T>& right)
    {
        x -= right.x;
        y -= right.y;
        z -= right.z;
        w -= right.w;
        return *this;
    }

    /// @brief Unary operator -= overload for scalar value
    /// @param scalar Scalar value to subtract from all elements
    /// @return The Vector4 result
    SKY_FORCE_INLINE Vector4<T>& operator-=(const T& scalar)
    {
        x -= scalar;
        y -= scalar;
        z -= scalar;
        w -= scalar;
        return *this;
    }

    /// @brief Unary operator *= overload for Vector4 value
    /// @param right Vector4 to multiply
    /// @return The Vector4 result
    SKY_FORCE_INLINE Vector4<T>& operator*=(const Vector4<T>& right)
    {
        x *= right.x;
        y *= right.y;
        z *= right.z;
        w *= right.w;
        return *this;
    }

    /// @brief Unary operator *= overload for scalar value
    /// @param scalar Scalar value to multiply with all elements
    /// @return The Vector4 result
    SKY_FORCE_INLINE Vector4<T>& operator*=(const T& scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    /// @brief Unary operator /= overload for Vector4 value
    /// @param right Vector4 to divide
    /// @return The Vector4 result
    SKY_FORCE_INLINE Vector4<T>& operator/=(const Vector4<T>& right)
    {
        x /= right.x;
        y /= right.y;
        z /= right.z;
        w /= right.w;
        return *this;
    }

    /// @brief Binary operator /= overload for scalar value
    /// @param scalar Scalar value to divide all elements by
    /// @return The Vector4 result
    SKY_FORCE_INLINE Vector4<T>& operator/=(const T& scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }

    /// @brief Operator[] overload for element access
    /// @param i The element index to access
    /// @return The element at index i
    SKY_FORCE_INLINE T& operator[](const int i)
    {
        SKY_ASSERT(i < 4, "Index out of bounds");

        return (&x)[i];
    }

    /// @brief Operator[] overload for const element access
    /// @param i The element index to access
    /// @return The element at index i
    SKY_FORCE_INLINE T const& operator[](const int i) const
    {
        SKY_ASSERT(i < 4, "Index out of bounds");

        return (&x)[i];
    }
};

//////////////////////////
// Non-Member overloads
//////////////////////////

// Arithmetic operators

/// @brief Binary operator+ overload with a Vector value
/// @tparam T The Vector4's data type
/// @param left Left operand
/// @param right Right operand
/// @return Result of addition of each of the lefts elements with the rights
template<typename T>
SKY_FORCE_INLINE Vector4<T> operator+(const Vector4<T>& left, const Vector4<T>& right)
{
    return Vector4<T>(
        left.x + right.x,
        left.y + right.y,
        left.z + right.z,
        left.w + right.w
    );
}

/// @brief Binary operator+ overload with a scalar value
/// @tparam T The Vector4 and scalars data type
/// @param vec Left operand
/// @param scalar Right operand (scalar)
/// @return Addition result of each of the vectors elements with the scalar.
template<typename T>
SKY_FORCE_INLINE Vector4<T> operator+(const Vector4<T>& vec, const T& scalar)
{
    return Vector4<T>(
        vec.x + scalar,
        vec.y + scalar,
        vec.z + scalar,
        vec.w + scalar
    );
}

/// @brief Binary operator- overload
/// @tparam T Data type of each Vector4
/// @param left Left operand
/// @param right Right operand
/// @return Subtraction result of each of the lefts elements by the right elements
template<typename T>
SKY_FORCE_INLINE Vector4<T> operator-(const Vector4<T>& left, const Vector4<T>& right)
{
    return Vector4<T>(
        left.x - right.x,
        left.y - right.y,
        left.z - right.z,
        left.w - right.w
    );
}

/// @brief Binary operator- overload with a scalar value
/// @tparam T Data type of the Vector4 and the scalar
/// @param left Left operand
/// @param right Right operand (scalar)
/// @return Subtraction result of each of the vectors elements by the scalar
template<typename T>
SKY_FORCE_INLINE Vector4<T> operator-(const Vector4<T>& vec, const T& scalar)
{
    return Vector4<T>(
        vec.x - scalar,
        vec.y - scalar,
        vec.z - scalar,
        vec.w - scalar
    );
}

/// @brief Binary operator/ overload
/// @tparam T Data type of each Vector4
/// @param left Left operand
/// @param right Right operand
/// @return Division result of each of the left operands elements over the right elements
template<typename T>
SKY_FORCE_INLINE Vector4<T> operator/(const Vector4<T>& left, const Vector4<T>& right)
{
    return Vector4<T>(
        left.x / right.x,
        left.y / right.y,
        left.z / right.z,
        left.w / right.w
    );
}

/// @brief Binary operator/ overload with a scalar value
/// @tparam T Data type of the Vector4 and the scalar
/// @param left Left operand
/// @param right Right operand
/// @return Division result of each of the left operands elements over the scalar
template<typename T>
SKY_FORCE_INLINE Vector4<T> operator/(const Vector4<T>& vec, const T& divisor)
{
    return Vector4<T>(
        vec.x / divisor,
        vec.y / divisor,
        vec.z / divisor,
        vec.w / divisor
    );
}

/// @brief Binary operator* overload
/// @tparam T Data type of the Vector4's
/// @param left The left operand
/// @param right The right operand
/// @return Multiplication result of each of the vectors elements
template<typename T>
SKY_FORCE_INLINE Vector4<T> operator*(const Vector4<T>& left, const Vector4<T>& right)
{
    return Vector4<T>(
        left.x * right.x,
        left.y * right.y,
        left.z * right.z,
        left.w * right.w
    );
}

/// @brief Binary operator* overload with a scalar value
/// @tparam T Data type of the Vector4 and the scalar
/// @param left The left operand
/// @param right The right operand
/// @return Multiplication result of each of the vectors elements with the scalar
template<typename T>
SKY_FORCE_INLINE Vector4<T> operator*(const Vector4<T>& vec, const T& scalar)
{
    return Vector4<T>(
        vec.x * scalar,
        vec.y * scalar,
        vec.z * scalar,
        vec.w * scalar
    );
}

template<typename T>
SKY_FORCE_INLINE Vector4<T> operator*(const T& scalar, const Vector4<T>& vec)
{
    return vec * scalar;
}

/// @brief Unary operator- overload (negation)
/// @tparam T Data type of the Vector4's elements
/// @param vec The vector to negate
/// @return The result of negating each of the vectors elements
template<typename T>
SKY_FORCE_INLINE Vector4<T> operator-(const Vector4<T>& vec)
{
    return Vector4<T>(
        -vec.x,
        -vec.y,
        -vec.z,
        -vec.w
    );
}

// Logical operators

/// @brief Binary operator== overload
/// @tparam T Data type of each Vector4
/// @param left Left operand
/// @param right Right operand
/// @return true if all elements in both Vector4's are equal, false otherwise
template<typename T>
SKY_FORCE_INLINE bool operator==(const Vector4<T>& left, const Vector4<T>& right)
{
    return (left.x == right.x) && (left.y == right.y) && (left.z == right.z)
        && (left.w == right.w);
}

/// @brief Binary operator!= overload
/// @tparam T Data type of each Vector4
/// @param left Left operand
/// @param right Right operand
/// @return true if any element in either Vector4 are not equal, false otherwise
template<typename T>
SKY_FORCE_INLINE bool operator!=(const Vector4<T>& left, const Vector4<T>& right)
{
    return (left.x != right.x) || (left.y != right.y) || (left.z != right.z)
        || (left.w != right.w);
}


/// @defgroup vec4aliases Vector4 Aliases
/// @{

/// @brief Type alias for a Vector4 with elements of type unsigned double
using Vector4d = Vector4<double>;
/// @brief Type alias for a Vector4 with elements of type unsigned float
using Vector4f = Vector4<float>;
/// @brief Type alias for a Vector4 with elements of type 32 bit unsigned int
using Vector4i = Vector4<int32_t>;
/// @brief Type alias for a Vector4 with elements of type 32 bit unsigned unsigned int
using Vector4u = Vector4<uint32_t>;
/// @brief Type alias for a Vector4 with elements of type unsigned bool
using Vector4b = Vector4<bool>;

/// @}

} // namespace sky

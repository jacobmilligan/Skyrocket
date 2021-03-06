//
//  Matrix.hpp
//  Skyrocket Framework
//
//  --------------------------------------------------------------
//
//  Created by Jacob Milligan on 10/12/2016.
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Vector3.hpp"
#include "Vector4.hpp"

#include <array>

namespace sky {

enum class ClipSpaceDepth {
    zero_to_one,
    negative_one_to_one
};

struct Matrix {
    static ClipSpaceDepth depth;
};

/// @brief Matrix4 is a 4x4 matrix representation with several useful utility
/// functions and operator overloads. Internally, it uses a column vectors and
/// utilizes the same functions and operator overloads defined on these Vector types
/// @tparam T The data type held within each matrix element. Supports any type that
/// has defined all arithmetic and logical operators
template<typename T>
struct Matrix4 : Matrix {
    /// @brief The column vector elements of the matrix
    T entries[16]{};

    /// @brief Default constructor - initializes the Matrix as the identity
    /// matrix, that is the main diagonal elements are initialized to 1
    /// with all other elements initialized to 0
    SKY_FORCE_INLINE Matrix4()
        : Matrix4(static_cast<T>(1))
    {}

    /// @brief Constructs a new matrix with the main diagonal elements
    /// initialized to the specified value
    /// @param value Value to initialize the matrix's main diagonal elements to
    SKY_FORCE_INLINE explicit Matrix4(const T value)
    {
        memset(entries, 0, sizeof(float) * 16);

        entries[0] = value;
        entries[5] = value;
        entries[10] = value;
        entries[15] = value;
    }

    /// @brief Constructs a new matrix with the specified column vectors used to
    /// initialize the matrix's column values from left to right
    /// @param col1 The left-most column
    /// @param col2 The left-center column
    /// @param col3 The right-center column
    /// @param col4 The right-most column
    SKY_FORCE_INLINE Matrix4(const Vector4 <T>& col1, const Vector4 <T>& col2,
            const Vector4 <T>& col3, const Vector4 <T>& col4)
    {
        entries[0] = col1.x;
        entries[1] = col1.y;
        entries[2] = col1.z;
        entries[3] = col1.w;

        entries[4] = col2.x;
        entries[5] = col2.y;
        entries[6] = col2.z;
        entries[7] = col2.w;

        entries[8] = col3.x;
        entries[9] = col3.y;
        entries[10] = col3.z;
        entries[11] = col3.w;

        entries[12] = col4.x;
        entries[13] = col4.y;
        entries[14] = col4.z;
        entries[15] = col4.w;
    }

    /// @brief Constructs a new matrix with the specified element values in
    /// column-major ordering, i.e. top to bottom, left to right
    /// @param x0
    /// @param y0
    /// @param z0
    /// @param w0
    /// @param x1
    /// @param y1
    /// @param z1
    /// @param w1
    /// @param x2
    /// @param y2
    /// @param z2
    /// @param w2
    /// @param x3
    /// @param y3
    /// @param z3
    /// @param w3
    SKY_FORCE_INLINE Matrix4(
        const T x0, const T x1, const T x2, const T x3,
        const T y0, const T y1, const T y2, const T y3,
        const T z0, const T z1, const T z2, const T z3,
        const T w0, const T w1, const T w2, const T w3
    )
    {
        entries[0] = x0;
        entries[1] = y0;
        entries[2] = z0;
        entries[3] = w0;

        entries[4] = x1;
        entries[5] = y1;
        entries[6] = z1;
        entries[7] = w1;

        entries[8] = x2;
        entries[9] = y2;
        entries[10] = z2;
        entries[11] = w2;

        entries[12] = x3;
        entries[13] = y3;
        entries[14] = z3;
        entries[15] = w3;
    }

    /// @brief Gets a string representation of the matrix
    /// @return The matrix string
    std::string to_string()
    {
        std::stringstream ss;

        ss << entries[0] << " " << entries[4] << " " << entries[8] << " " << entries[12] << "\n"
           << entries[1] << " " << entries[5] << " " << entries[9] << " " << entries[13] << "\n"
           << entries[2] << " " << entries[6] << " " << entries[10] << " " << entries[14] << "\n"
           << entries[3] << " " << entries[7] << " " << entries[11] << " " << entries[15] << "\n";

        return ss.str();
    }

    /// @brief Defines subscript operator for accessing the column vector elements
    /// of the matrix
    /// @param i The column vector to access
    /// @return The column vector
    SKY_FORCE_INLINE T* operator[](const int i)
    {
        return &entries[i * 4];
    }

    /// @brief Defines subscript operator for accessing the column vector elements
    /// of the matrix. Definition used for const accessing
    /// @param i The column vector to access
    /// @return The column vector
    SKY_FORCE_INLINE const T* operator[](const int i) const
    {
        return &entries[i * 4];
    }

    /// @brief Gets a scale matrix using this matrix and a column Vector3 as a basis
    /// @param v The column vector to scale the matrix by
    /// @return The scale matrix
    SKY_FORCE_INLINE static Matrix4<T> scale(const Vector3<T>& v)
    {
        Matrix4<T> result;

        result.entries[0] *= v.x;
        result.entries[5] *= v.y;
        result.entries[10] *= v.z;

        return result;
    }

    /// @brief Gets a translation matrix using this matrix and a
    /// column Vector3 as a basis
    /// @param v The column vector to translate the matrix by
    /// @return The translation matrix
    SKY_FORCE_INLINE static Matrix4<T> translate(const Vector3<T>& v)
    {
        Matrix4<T> trans;

        trans.entries[12] = v.x;
        trans.entries[13] = v.y;
        trans.entries[14] = v.z;
        return trans;
    }

    /// @brief Transposes the matrix
    /// @return The matrix transpose
    SKY_FORCE_INLINE static Matrix4<T> transpose(Matrix4<T>& mat)
    {
        auto result = mat;

        result.entries[1] = mat.entries[4];
        result.entries[2] = mat.entries[8];
        result.entries[3] = mat.entries[12];

        result.entries[4] = mat.entries[1];
        result.entries[5] = mat.entries[5];
        result.entries[6] = mat.entries[9];
        result.entries[7] = mat.entries[13];

        result.entries[8] = mat.entries[2];
        result.entries[9] = mat.entries[6];
        result.entries[10] = mat.entries[10];
        result.entries[11] = mat.entries[14];

        result.entries[12] = mat.entries[3];
        result.entries[13] = mat.entries[7];
        result.entries[14] = mat.entries[11];
        result.entries[15] = mat.entries[15];

        return result;
    }

    /// @brief Gets a rotation matrix using this matrix and a Vector3 that describes
    /// the axis to rotate around, specifying the angle in degrees to rotate with.
    /// @param theta The angle at which to rotate
    /// @param axis The vector representing an axis to rotate around
    /// @return The rotation matrix
    SKY_FORCE_INLINE static Matrix4<T> rotate(const T theta, const Vector3<T>& axis)
    {
        Matrix4<T> rotation;

        auto omega = axis.get_normalized();
        auto costheta = static_cast<T>(cos(theta));
        auto sintheta = static_cast<T>(sin(theta));
        auto one = static_cast<T>(1);
        auto omega_xsquared = (omega.x * omega.x);
        auto omega_ysquared = (omega.y * omega.y);
        auto omega_zsquared = (omega.z * omega.z);

        rotation.entries[0] = costheta + omega_xsquared * (one - costheta);
        rotation.entries[1] = omega.z * sintheta + omega.x * omega.y * (one - costheta);
        rotation.entries[2] = -omega.y * sintheta + omega.x * omega.z * (one - costheta);

        rotation.entries[4] = omega.x * omega.y * (one - costheta) - omega.z * sintheta;
        rotation.entries[5] = costheta + omega_ysquared * (one - costheta);
        rotation.entries[6] = -omega.x * sintheta + omega.y * omega.z * (one - costheta);

        rotation.entries[8] = omega.y * sintheta + omega.x * omega.z * (one - costheta);
        rotation.entries[9] = omega.x * sintheta + omega.y * omega.z * (one - costheta);
        rotation.entries[10] = costheta + omega_zsquared * (one - costheta);

        return rotation;
    }

    /// @brief Creates a left-handed orthographic projection matrix used for
    /// projecting 3D world coordinates to 2D screen space
    /// @param left The left-most point of the viewing area
    /// @param right The right-most point of the viewing area
    /// @param bottom The bottom-most point of the viewing area
    /// @param top The top-most point of the viewing area
    /// @param near The closest point of the viewing area
    /// @param far The furthest point of the viewing area
    /// @return The orthographic projection matrix
    static Matrix4<T> ortho(const T left, const T right, const T bottom,
                     const T top, const T near, const T far)
    {
        Matrix4<T> result;

        auto size_x = static_cast<T>(right - left);
        auto size_y = static_cast<T>(top - bottom);
        auto zoom_x = static_cast<T>(2) / size_x;
        auto zoom_y = static_cast<T>(2) / size_y;

        auto a = static_cast<T>(1);

        result.entries[0] = zoom_x;
        result.entries[5] = zoom_y;
        result.entries[10] = a / (far - near);
        result.entries[12] = -(right + left) / size_x;
        result.entries[13] = -(top + bottom) / size_y;
        result.entries[14] = -near / (far - near);

        return result;
    }

    /// @brief Creates a perspective projection matrix
    /// @param fov_y
    /// @param aspect generally view.width / view.height
    /// @param z_near
    /// @param z_far
    /// @return
    static Matrix4<T> perspective(const T fov_y, const T aspect, const T z_near, const T z_far)
    {
        Matrix4<T> result;

        auto a = (depth == ClipSpaceDepth::zero_to_one) ? -(z_far + z_near) : z_far;
        auto b = (depth == ClipSpaceDepth::zero_to_one) ? -(static_cast<T>(2) * z_far * z_near)
                                                        : -(z_far * z_near);
        auto c = (depth == ClipSpaceDepth::zero_to_one) ? static_cast<T>(-1) : static_cast<T>(1);
        auto d = (depth == ClipSpaceDepth::zero_to_one) ? static_cast<T>(1) : static_cast<T>(0);

        auto y_scale = static_cast<T>(1) / tan(fov_y * 0.5);

        result.entries[0] = y_scale / aspect;
        result.entries[5] = y_scale;

        result.entries[10] = a / (z_far - z_near);
        result.entries[11] = c;
        result.entries[14] = b / (z_far - z_near);
        result.entries[15] = d;


        return result;
    }

    /// @brief Constructs a matrix to use as a view matrix for looking at a
    /// particular point in 3D space
    /// @param eye The viewers eye point
    /// @param target The new point in 3D space to look at
    /// @param up The vector pointing upwards in 3D space relative to the eye point
    /// @return The look_at matrix
    static Matrix4<T> look_at(const Vector3 <T>& eye, const Vector3 <T>& target,
                       const Vector3 <T>& up)
    {
        Matrix4<T> result;

        auto center = (depth == ClipSpaceDepth::zero_to_one) ? eye - target : target - eye;
        auto sign = (depth == ClipSpaceDepth::zero_to_one) ? static_cast<T>(1) : static_cast<T>(-1);

        auto zaxis = center.get_normalized();
        auto xaxis = (sign * up).cross(zaxis).get_normalized();
        auto yaxis = (sign * zaxis).cross(xaxis); // pointing up

        // Up
        result.entries[0] = xaxis.x; // xaxis.x
        result.entries[1] = yaxis.x; // xaxis.y
        result.entries[2] = zaxis.x; // xaxis.z
        result.entries[4] = xaxis.y; // yaxis.x
        result.entries[5] = yaxis.y; // yaxis.y
        result.entries[6] = zaxis.y; // yaxis.z
        result.entries[8] = xaxis.z; // zaxis.x
        result.entries[9] = yaxis.z; // zaxis.y
        result.entries[10] = zaxis.z; // zaxis.z

        // Calculate translations - always in 4th column
        result.entries[12] = -xaxis.dot(eye); // -position.x
        result.entries[13] = -yaxis.dot(eye); // -position.y
        result.entries[14] = -zaxis.dot(eye); // z for rhs instead of -z

        return result;
    }
};

/// @brief Operator* overload for multiplying a matrix by a column vector and
/// retrieving a vector result
/// @tparam T Data type of the matrix and vector
/// @param mat The matrix to multiply
/// @param vec The vector to multiple
/// @return The Vector result of the multiplication
template<typename T>
SKY_FORCE_INLINE Vector4 <T> operator*(const Matrix4<T>& mat, const Vector4 <T>& vec)
{
    return Vector4<T>(
        mat.entries[0] * vec[0] + mat.entries[1] * vec[0] + mat.entries[2] * vec[0] + mat.entries[3] * vec[0],
        mat.entries[4] * vec[1] + mat.entries[5] * vec[1] + mat.entries[6] * vec[1] + mat.entries[7] * vec[1],
        mat.entries[8] * vec[2] + mat.entries[9] * vec[2] + mat.entries[10] * vec[2] + mat.entries[11] * vec[2],
        mat.entries[12] * vec[3] + mat.entries[13] * vec[3] + mat.entries[14] * vec[3] + mat.entries[15] * vec[3]
    );
}

/// @brief Operator* overload for multiplying a Matrix4 by a Matrix4
/// @tparam T Data type of the matrices
/// @param left LHS Matrix
/// @param right RHS Matrix
/// @return The result of the multiplication operation
template<typename T>
SKY_FORCE_INLINE Matrix4<T> operator*(const Matrix4<T>& left, const Matrix4<T>& right)
{
    auto result = left;
    auto* A = left.entries;
    auto* B = right.entries;

    // Column 1
    result.entries[0] = B[0] * A[0] + B[1] * A[4] + B[2] * A[8] + B[3] * A[12];
    result.entries[1] = B[0] * A[1] + B[1] * A[5] + B[2] * A[9] + B[3] * A[13];
    result.entries[2] = B[0] * A[2] + B[1] * A[6] + B[2] * A[10] + B[3] * A[14];
    result.entries[3] = B[0] * A[3] + B[1] * A[7] + B[2] * A[11] + B[3] * A[15];

    // Column 2
    result.entries[4] = B[4] * A[0] + B[5] * A[4] + B[6] * A[8] + B[7] * A[12];
    result.entries[5] = B[4] * A[1] + B[5] * A[5] + B[6] * A[9] + B[7] * A[13];
    result.entries[6] = B[4] * A[2] + B[5] * A[6] + B[6] * A[10] + B[7] * A[14];
    result.entries[7] = B[4] * A[3] + B[5] * A[7] + B[6] * A[11] + B[7] * A[15];

    // Column 3
    result.entries[8] =  B[8] * A[0] + B[9] * A[4] + B[10] * A[8] + B[11] * A[12];
    result.entries[9] =  B[8] * A[1] + B[9] * A[5] + B[10] * A[9] + B[11] * A[13];
    result.entries[10] = B[8] * A[2] + B[9] * A[6] + B[10] * A[10] + B[11] * A[14];
    result.entries[11] = B[8] * A[3] + B[9] * A[7] + B[10] * A[11] + B[11] * A[15];

    // Column 4
    result.entries[12] =  B[12] * A[0] + B[13] * A[4] + B[14] * A[8] + B[15] * A[12];
    result.entries[13] =  B[12] * A[1] + B[13] * A[5] + B[14] * A[9] + B[15] * A[13];
    result.entries[14] =  B[12] * A[2] + B[13] * A[6] + B[14] * A[10] + B[15] * A[14];
    result.entries[15] =  B[12] * A[3] + B[13] * A[7] + B[14] * A[11] + B[15] * A[15];

    return result;
}

/// @defgroup mat4aliases Vector3 Aliases
/// @{

/// @brief Type alias for a Matrix4 with elements of type double
using Matrix4d = Matrix4<double>;
/// @brief Type alias for a Matrix4 with elements of type float
using Matrix4f = Matrix4<float>;
/// @brief Type alias for a Matrix4 with elements of type int
using Matrix4i = Matrix4<int>;
/// @brief Type alias for a Matrix4 with elements of type unsigned int
using Matrix4u = Matrix4<unsigned int>;
/// @brief Type alias for a Matrix4 with elements of type bool
using Matrix4b = Matrix4<bool>;

/// @}


}  // namespace sky

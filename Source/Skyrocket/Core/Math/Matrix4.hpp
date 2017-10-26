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


/// @brief Matrix4 is a 4x4 matrix representation with several useful utility
/// functions and operator overloads. Internally, it uses a column vectors and
/// utilizes the same functions and operator overloads defined on these Vector types
/// @tparam T The data type held within each matrix element. Supports any type that
/// has defined all arithmetic and logical operators
template<typename T>
struct Matrix4 {
    /// @brief The column vector elements of the matrix
    float columns[16]{};

    /// @brief Default constructor - initializes the Matrix as the identity
    /// matrix, that is the main diagonal elements are initialized to 1
    /// with all other elements initialized to 0
    Matrix4()
        : Matrix4(static_cast<T>(1))
    {}

    /// @brief Constructs a new matrix with the main diagonal elements
    /// initialized to the specified value
    /// @param value Value to initialize the matrix's main diagonal elements to
    explicit Matrix4(const T value)
    {
        for ( auto& c : columns ) {
            c = static_cast<T>(0);
        }

        columns[0] = value;
        columns[5] = value;
        columns[10] = value;
        columns[15] = value;
    }

    /// @brief Constructs a new matrix with the specified column vectors used to
    /// initialize the matrix's column values from left to right
    /// @param col1 The left-most column
    /// @param col2 The left-center column
    /// @param col3 The right-center column
    /// @param col4 The right-most column
    Matrix4(const Vector4 <T>& col1, const Vector4 <T>& col2,
            const Vector4 <T>& col3, const Vector4 <T>& col4)
    {
        columns[0] = col1.x;
        columns[1] = col1.y;
        columns[2] = col1.z;
        columns[3] = col1.w;

        columns[4] = col2.x;
        columns[5] = col2.y;
        columns[6] = col2.z;
        columns[7] = col2.w;

        columns[8] = col3.x;
        columns[9] = col3.y;
        columns[10] = col3.z;
        columns[11] = col3.w;

        columns[12] = col4.x;
        columns[13] = col4.y;
        columns[14] = col4.z;
        columns[15] = col4.w;
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
    Matrix4(
        const T x0, const T x1, const T x2, const T x3,
        const T y0, const T y1, const T y2, const T y3,
        const T z0, const T z1, const T z2, const T z3,
        const T w0, const T w1, const T w2, const T w3
    )
    {
        columns[0] = x0;
        columns[1] = y0;
        columns[2] = z0;
        columns[3] = w0;

        columns[4] = x1;
        columns[5] = y1;
        columns[6] = z1;
        columns[7] = w1;

        columns[8] = x2;
        columns[9] = y2;
        columns[10] = z2;
        columns[11] = w2;

        columns[12] = x3;
        columns[13] = y3;
        columns[14] = z3;
        columns[15] = w3;
    }

    /// @brief Gets a string representation of the matrix
    /// @return The matrix string
    std::string to_string()
    {
        std::stringstream ss;

        ss << columns[0] << " " << columns[4] << " " << columns[8] << " " << columns[12] << "\n"
           << columns[1] << " " << columns[5] << " " << columns[9] << " " << columns[13] << "\n"
           << columns[2] << " " << columns[6] << " " << columns[10] << " " << columns[14] << "\n"
           << columns[3] << " " << columns[7] << " " << columns[11] << " " << columns[15] << "\n";

        return ss.str();
    }

    /// @brief Gets a scale matrix using this matrix and a column Vector3 as a basis
    /// @param v The column vector to scale the matrix by
    /// @return The scale matrix
    Matrix4<T> scale(const Vector3<T>& v)
    {
        auto result = *this;

        result.columns[0] *= v.x;
        result.columns[5] *= v.y;
        result.columns[10] *= v.z;

        return result;
    }

    /// @brief Gets a translation matrix using this matrix and a
    /// column Vector3 as a basis
    /// @param v The column vector to translate the matrix by
    /// @return The translation matrix
    Matrix4<T> translate(const Vector3<T>& v)
    {
        Matrix4<T> trans = *this;
        trans.columns[12] = v.x;
        trans.columns[13] = v.y;
        trans.columns[14] = v.z;
        return trans;
    }

    /// @brief Transposes the matrix
    /// @return The matrix transpose
    Matrix4<T> transpose()
    {
        auto start = *this;
        auto result = start;

        result.columns[1] = columns[4];
        result.columns[2] = columns[8];
        result.columns[3] = columns[12];

        result.columns[4] = columns[1];
        result.columns[5] = columns[5];
        result.columns[6] = columns[9];
        result.columns[7] = columns[13];

        result.columns[8] = columns[2];
        result.columns[9] = columns[6];
        result.columns[10] = columns[10];
        result.columns[11] = columns[14];

        result.columns[12] = columns[3];
        result.columns[13] = columns[7];
        result.columns[14] = columns[11];
        result.columns[15] = columns[15];

        return result;
    }

    /// @brief Gets a rotation matrix using this matrix and a Vector3 that describes
    /// the axis to rotate around, specifying the angle in degrees to rotate with.
    /// @param theta The angle at which to rotate
    /// @param axis The vector representing an axis to rotate around
    /// @return The rotation matrix
    Matrix4<T> rotate(const T theta, const Vector3<T>& axis)
    {
        auto rotation = *this;

        auto omega = axis.get_normalized();
        auto costheta = static_cast<T>(cos(theta));
        auto sintheta = static_cast<T>(sin(theta));
        auto one = static_cast<T>(1);
        auto omega_xsquared = (omega.x * omega.x);
        auto omega_ysquared = (omega.y * omega.y);
        auto omega_zsquared = (omega.z * omega.z);

//        auto dsin = static_cast<T>(sin(angle));
//        auto dcos = static_cast<T>(cos(angle));
//        auto ocos = static_cast<T>(1.0 - dcos);
//
//        // Rodrigues' Rotation Formula
//        rotation[0][0] = dcos + (omega.x * omega.x) * ocos;
//        rotation[0][1] = omega.y * omega.x + omega.z * dsin;
//        rotation[0][2] = omega.z * omega.x * ocos - omega.y * dsin;
//
//        rotation[1][0] = omega.x * omega.y * ocos - omega.z * dsin;
//        rotation[1][1] = dcos + (omega.y * omega.y) * ocos;
//        rotation[1][2] = omega.z * omega.y * ocos - omega.x * dsin;
//
//        rotation[2][0] = omega.x * omega.z * ocos + omega.y * dsin;
//        rotation[2][1] = omega.y * omega.z * ocos + omega.x * dsin;
//        rotation[2][2] = dcos + (omega.z * omega.z) * ocos;

        rotation.columns[0] = costheta + omega_xsquared * (one - costheta);
        rotation.columns[1] = omega.z * sintheta + omega.x * omega.y * (one - costheta);
        rotation.columns[2] = -omega.y * sintheta + omega.x * omega.z * (one - costheta);

        rotation.columns[4] = omega.x * omega.y * (one - costheta) - omega.z * sintheta;
        rotation.columns[5] = costheta + omega_ysquared * (one - costheta);
        rotation.columns[6] = omega.x * sintheta + omega.y * omega.z * (one - costheta);

        rotation.columns[8] = omega.y * sintheta + omega.x * omega.z * (one - costheta);
        rotation.columns[9] = -omega.x * sintheta + omega.y * omega.z * (one - costheta);
        rotation.columns[10] = costheta * omega_zsquared * (one - costheta);

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
    Matrix4<T> ortho(const T left, const T right, const T bottom,
                     const T top, const T near, const T far)
    {
        Matrix4<T> result;

        // Width and height
        result.columns[0] = static_cast<T>(2) / (right - left);
        result.columns[5] = static_cast<T>(2) / (top - bottom);
#if SKY_GRAPHICS_API_OPENGL == 1
        // OpenGL expects a lhs for the perspective divide stage
        // so the z coordinate needs to be negated to flip it
        result.columns[10] = static_cast<T>(-2) / (far - near);
        result.columns[12] = -(right + left) / (right - left);
        result.columns[13] = -(top + bottom) / (top - bottom);
        result.columns[14] = -(far + near) / (far - near); // f + n instead of just n in numer for gl compatibility
#elif SKY_GRAPHICS_API_METAL == 1
        auto mtl_near = static_cast<T>(0.0);

        result.columns[10] = static_cast<T>(1) / (far - mtl_near);
        result.columns[12] = (right + left) / (left - right);
        result.columns[13] = (top + bottom) / (bottom - top);
        result.columns[14] = mtl_near / (far - mtl_near);
#endif

        return result;
    }

    /// @brief Creates a perspective projection matrix
    /// @param fov_y
    /// @param aspect generally view.width / view.height
    /// @param z_near
    /// @param z_far
    /// @return
    Matrix4<T> perspective(const T fov_y, const T aspect, const T z_near, const T z_far)
    {
        Matrix4<T> result(0);

        auto y_scale = static_cast<T>(1) / tan(fov_y * 0.5);

#if SKY_GRAPHICS_API_METAL == 1
        auto x_scale = y_scale / aspect;
        auto q = -(z_far + z_near) / (z_far - z_near);

        result.columns[0] = x_scale;
        result.columns[5] = y_scale;

        result.columns[10] = q;
        result.columns[11] = static_cast<T>(-1);
        result.columns[14] = static_cast<T>(-2) * z_far * z_near / (z_far - z_near);
#endif

        return result;
    }

    /// @brief Constructs a matrix to use as a view matrix for looking at a
    /// particular point in 3D space
    /// @param eye The viewers eye point
    /// @param target The new point in 3D space to look at
    /// @param up The vector pointing upwards in 3D space relative to the eye point
    /// @return The look_at matrix
    Matrix4<T> look_at(const Vector3 <T>& eye, const Vector3 <T>& target,
                       const Vector3 <T>& up)
    {
        Matrix4<T> result;

        Vector3<T> f(target - eye); // towards center
        f.normalize();

        auto s = f.cross(up);
        s.normalize();

        auto u = s.cross(f); // pointing up

        // Up
        result.columns[1] = u.x;
        result.columns[5] = u.y;
        result.columns[9] = u.z;

        // Right
        result.columns[0] = s.x;
        result.columns[4] = s.y;
        result.columns[8] = s.z;

        // Negative target for rhs
        result.columns[2] = -f.x;
        result.columns[6] = -f.y;
        result.columns[10] = -f.z;

        // Calculate translations - always in 4th column
        result.columns[12] = -s.dot(eye); // -position.x
        result.columns[13] = -u.dot(eye); // -position.y
        result.columns[14] = f.dot(eye); // z for rhs instead of -z

        return result;
    }

    /// @brief Defines subscript operator for accessing the column vector elements
    /// of the matrix
    /// @param i The column vector to access
    /// @return The column vector
    T* operator[](const int i)
    {
        return &columns[i];
    }

    /// @brief Defines subscript operator for accessing the column vector elements
    /// of the matrix. Definition used for const accessing
    /// @param i The column vector to access
    /// @return The column vector
    const T* operator[](const int i) const
    {
        return &columns[i];
    }
};

/// @brief Operator* overload for multiplying a matrix by a column vector and
/// retrieving a vector result
/// @tparam T Data type of the matrix and vector
/// @param mat The matrix to multiply
/// @param vec The vector to multiple
/// @return The Vector result of the multiplication
template<typename T>
Vector4 <T> operator*(const Matrix4<T>& mat, const Vector4 <T>& vec)
{
    return Vector4<T>(
        mat.columns[0] * vec[0] + mat.columns[1] * vec[0] + mat.columns[2] * vec[0] + mat.columns[3] * vec[0],
        mat.columns[4] * vec[1] + mat.columns[5] * vec[1] + mat.columns[6] * vec[1] + mat.columns[7] * vec[1],
        mat.columns[8] * vec[2] + mat.columns[9] * vec[2] + mat.columns[10] * vec[2] + mat.columns[11] * vec[2],
        mat.columns[12] * vec[3] + mat.columns[13] * vec[3] + mat.columns[14] * vec[3] + mat.columns[15] * vec[3]
    );
}

/// @brief Operator* overload for multiplying a Matrix4 by a Matrix4
/// @tparam T Data type of the matrices
/// @param left LHS Matrix
/// @param right RHS Matrix
/// @return The result of the multiplication operation
template<typename T>
Matrix4<T> operator*(const Matrix4<T>& left, const Matrix4<T>& right)
{
    Matrix4<T> result;

//    auto& A1 = left[0];
//    auto& A2 = left[1];
//    auto& A3 = left[2];
//    auto& A4 = left[3];
//
//    auto& B1 = right[0];
//    auto& B2 = right[1];
//    auto& B3 = right[2];
//    auto& B4 = right[3];
//
//    result[0] = A1 * B1[0] + A2 * B1[1] + A3 * B1[2] + A4 * B1[3];
//    result[1] = A1 * B2[0] + A2 * B2[1] + A3 * B2[2] + A4 * B2[3];
//    result[2] = A1 * B3[0] + A2 * B3[1] + A3 * B3[2] + A4 * B3[3];
//    result[3] = A1 * B4[0] + A2 * B4[1] + A3 * B4[2] + A4 * B4[3];

    result.columns[0] = right.columns[0] * left.columns[0] + right.columns[1] * left.columns[4] + right.columns[2] * left.columns[8] + right.columns[3] * left.columns[12];
    result.columns[1] = right.columns[0] * left.columns[1] + right.columns[1] * left.columns[5] + right.columns[2] * left.columns[9] + right.columns[3] * left.columns[13];
    result.columns[2] = right.columns[0] * left.columns[2] + right.columns[1] * left.columns[6] + right.columns[2] * left.columns[10] + right.columns[3] * left.columns[14];
    result.columns[3] = right.columns[0] * left.columns[3] + right.columns[1] * left.columns[7] + right.columns[2] * left.columns[11] + right.columns[3] * left.columns[15];

    result.columns[4] = right.columns[4] * left.columns[0] + right.columns[5] * left.columns[4] + right.columns[6] * left.columns[8] + right.columns[7] * left.columns[12];
    result.columns[5] = right.columns[4] * left.columns[1] + right.columns[5] * left.columns[5] + right.columns[6] * left.columns[9] + right.columns[7] * left.columns[13];
    result.columns[6] = right.columns[4] * left.columns[2] + right.columns[5] * left.columns[6] + right.columns[6] * left.columns[10] + right.columns[7] * left.columns[14];
    result.columns[7] = right.columns[4] * left.columns[3] + right.columns[5] * left.columns[7] + right.columns[6] * left.columns[11] + right.columns[7] * left.columns[15];

    result.columns[8] =  right.columns[8] * left.columns[0] + right.columns[9] * left.columns[4] + right.columns[10] * left.columns[8] + right.columns[11] * left.columns[12];
    result.columns[9] =  right.columns[8] * left.columns[1] + right.columns[9] * left.columns[5] + right.columns[10] * left.columns[9] + right.columns[11] * left.columns[13];
    result.columns[10] = right.columns[8] * left.columns[2] + right.columns[9] * left.columns[6] + right.columns[10] * left.columns[10] + right.columns[11] * left.columns[14];
    result.columns[11] = right.columns[8] * left.columns[3] + right.columns[9] * left.columns[7] + right.columns[10] * left.columns[11] + right.columns[11] * left.columns[15];

    result.columns[12] =  right.columns[12] * left.columns[0] + right.columns[13] * left.columns[4] + right.columns[14] * left.columns[8] + right.columns[15] * left.columns[12];
    result.columns[13] =  right.columns[12] * left.columns[1] + right.columns[13] * left.columns[5] + right.columns[14] * left.columns[9] + right.columns[15] * left.columns[13];
    result.columns[14] =  right.columns[12] * left.columns[2] + right.columns[13] * left.columns[6] + right.columns[14] * left.columns[10] + right.columns[15] * left.columns[14];
    result.columns[15] =  right.columns[12] * left.columns[3] + right.columns[13] * left.columns[7] + right.columns[14] * left.columns[11] + right.columns[15] * left.columns[15];

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

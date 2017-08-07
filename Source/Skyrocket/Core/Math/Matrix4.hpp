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

#include <array>

#include "Vector3.hpp"
#include "Vector4.hpp"

namespace sky {


/// @brief Matrix4 is a 4x4 matrix representation with several useful utility
/// functions and operator overloads. Internally, it uses a column vectors and
/// utilizes the same functions and operator overloads defined on these Vector types
/// @tparam T The data type held within each matrix element. Supports any type that
/// has defined all arithmetic and logical operators
template <typename T>
struct Matrix4 {
    /// @brief The column vector elements of the matrix
    Vector4<T> columns[4];

    /// @brief Default constructor - initializes the Matrix as the identity
    /// matrix, that is the main diagonal elements are initialized to 1
    /// with all other elements initialized to 0
    Matrix4() : Matrix4(static_cast<T>(1)) {}

    /// @brief Constructs a new matrix with the main diagonal elements
    /// initialized to the specified value
    /// @param value Value to initialize the matrix's main diagonal elements to
    Matrix4(const T value)
    {
        columns[0] = Vector4<T>(value, 0, 0, 0);
        columns[1] = Vector4<T>(0, value, 0, 0);
        columns[2] = Vector4<T>(0, 0, value, 0);
        columns[3] = Vector4<T>(0, 0, 0, value);
    }
    
    /// @brief Constructs a new matrix with the specified column vectors used to
    /// initialize the matrix's column values from left to right
    /// @param col1 The left-most column
    /// @param col2 The left-center column
    /// @param col3 The right-center column
    /// @param col4 The right-most column
    Matrix4(const Vector4<T>& col1, const Vector4<T>& col2,
                     const Vector4<T>& col3, const Vector4<T>& col4)
    {
        columns[0] = col1;
        columns[1] = col2;
        columns[2] = col3;
        columns[3] = col4;
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
        columns[0] = Vector4<T>(x0, y0, z0, w0);
        columns[1] = Vector4<T>(x1, y1, z1, w1);
        columns[2] = Vector4<T>(x2, y2, z2, w2);
        columns[3] = Vector4<T>(x3, y3, z3, w3);
    }
    
    /// @brief Gets a string representation of the matrix
    /// @return The matrix string
    std::string to_string()
    {
        std::stringstream ss;
        
        ss << columns[0].to_string() << "\n"
            << columns[1].to_string() << "\n"
            << columns[2].to_string() << "\n"
            << columns[3].to_string() << "\n";
        
        return ss.str();
    }
    
    /// @brief Gets a scale matrix using this matrix and a column Vector3 as a basis
    /// @param delta The column vector to scale the matrix by
    /// @return The scale matrix
    Matrix4<T> scale(const Vector3<T>& delta)
    {
        auto result = *this;

        result[0] *= delta.x;
        result[1] *= delta.y;
        result[2] *= delta.z;

        return result;
    }
    
    /// @brief Gets a translation matrix using this matrix and a
    /// column Vector3 as a basis
    /// @param delta The column vector to translate the matrix by
    /// @return The translation matrix
    Matrix4<T> translate(const Vector3<T>& delta)
    {
        Matrix4<T> trans = *this;
        trans[3][0] = delta.x;
        trans[3][1] = delta.y;
        trans[3][2] = delta.z;
        return trans;
    }

    Matrix4<T> transpose()
    {
        auto start = *this;
        auto result = start;

        result[0][0] = start[0][0];
        result[0][1] = start[1][0];
        result[0][2] = start[2][0];
        result[0][3] = start[3][0];

        result[1][0] = start[0][1];
        result[1][1] = start[1][1];
        result[1][2] = start[2][1];
        result[1][3] = start[3][1];

        result[2][0] = start[0][2];
        result[2][1] = start[1][2];
        result[2][2] = start[2][2];
        result[2][3] = start[3][2];

        result[3][0] = start[0][3];
        result[3][1] = start[1][3];
        result[3][2] = start[2][3];
        result[3][3] = start[3][3];

        return result;
    }
    
    /// @brief Gets a rotation matrix using this matrix and a Vector3 that describes
    /// the axis to rotate around, specifying the angle in degrees to rotate with.
    /// @param angle The angle at which to rotate
    /// @param axis_vec The vector representing an axis to rotate around
    /// @return The rotation matrix
    Matrix4<T> rotate(const T angle, const Vector3<T>& axis_vec)
    {
        auto axis = axis_vec;
        auto rotation = *this;
        
        axis.normalize();
        
        auto dsin = static_cast<T>(sin(angle));
        auto dcos = static_cast<T>(cos(angle));
        auto ocos = static_cast<T>(1.0 - dcos);
        
        
        // Rodrigues' Rotation Formula
        rotation[0][0] = dcos + (axis.x * axis.x) * ocos;
        rotation[0][1] = axis.y * axis.x + axis.z * dsin;
        rotation[0][2] = axis.z * axis.x * ocos - axis.y * dsin;
        
        rotation[1][0] = axis.x * axis.y * ocos - axis.z * dsin;
        rotation[1][1] = dcos + (axis.y * axis.y) * ocos;
        rotation[1][2] = axis.z * axis.y * ocos + axis.x * dsin;
        
        rotation[2][0] = axis.x * axis.z * ocos + axis.y * dsin;
        rotation[2][1] = axis.y * axis.z * ocos - axis.x * dsin;
        rotation[2][2] = dcos + (axis.z * axis.z) * ocos;
        
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
        result[0][0] = static_cast<T>(2) / (right - left);
        result[1][1] = static_cast<T>(2) / (top - bottom);
#if SKY_GRAPHICS_API_OPENGL == 1
        // OpenGL expects a lhs for the perspective divide stage
        // so the z coordinate needs to be negated to flip it
        result[2][2] = static_cast<T>(-2) / (far - near);
        result[3][2] = -(far + near) / (far - near); // f + n instead of just n in numer for gl compatibility

        result[3][0] = -(right + left) / (right - left);
        result[3][1] = -(top + bottom) / (top - bottom);
#elif SKY_GRAPHICS_API_METAL == 1
        auto mtl_near = static_cast<T>(0.0);
        result[2][2] = static_cast<T>(1) / (far - mtl_near);

        result[3][0] = (right + left) / (left - right);
        result[3][1] = (top + bottom) / (bottom - top);
        result[3][2] = mtl_near / (far - mtl_near);
#endif
        
        return result;
    }
    
    /// @brief Constructs a matrix to use as a view matrix for looking at a
    /// particular point in 3D space
    /// @param eye The viewers eye point
    /// @param target The new point in 3D space to look at
    /// @param up The vector pointing upwards in 3D space relative to the eye point
    /// @return The look_at matrix
    Matrix4<T> look_at(const Vector3<T>& eye, const Vector3<T>& target,
                       const Vector3<T>& up)
    {
        Matrix4<T> result;
        
        Vector3<T> f(target - eye); // towards center
        f.normalize();
        
        auto s = f.cross(up);
        s.normalize();
        
        auto u = s.cross(f); // pointing up
        
        // Up
        result[0][1] = u.x;
        result[1][1] = u.y;
        result[2][1] = u.z;
        
        // Right
        result[0][0] = s.x;
        result[1][0] = s.y;
        result[2][0] = s.z;
        
        // Negative target for rhs
        result[0][2] = -f.x;
        result[1][2] = -f.y;
        result[2][2] = -f.z;
        
        // Calculate translations - always in 4th column
        result[3][0] = -s.dot(eye); // -position.x
        result[3][1] = -u.dot(eye); // -position.y
        result[3][2] = f.dot(eye); // z for rhs instead of -z
        
        return result;
    }
    
    /// @brief Defines subscript operator for accessing the column vector elements
    /// of the matrix
    /// @param i The column vector to access
    /// @return The column vector
    Vector4<T>& operator[](const int i)
    {
        return columns[i];
    }
    
    /// @brief Defines subscript operator for accessing the column vector elements
    /// of the matrix. Definition used for const accessing
    /// @param i The column vector to access
    /// @return The column vector
    const Vector4<T>& operator[](const int i) const
    {
        return columns[i];
    }
};

/// @brief Operator* overload for multiplying a matrix by a column vector and
/// retrieving a vector result
/// @tparam T Data type of the matrix and vector
/// @param mat The matrix to multiply
/// @param vec The vector to multiple
/// @return The Vector result of the multiplication
template <typename T>
Vector4<T> operator*(const Matrix4<T>& mat, const Vector4<T>& vec)
{
    return Vector4<T>(
        mat[0][0] * vec[0] + mat[0][1] * vec[0] + mat[0][2] * vec[0] + mat[0][3] * vec[0],
        mat[1][0] * vec[1] + mat[1][1] * vec[1] + mat[1][2] * vec[1] + mat[1][3] * vec[1],
        mat[2][0] * vec[2] + mat[2][1] * vec[2] + mat[2][2] * vec[2] + mat[2][3] * vec[2],
        mat[3][0] * vec[3] + mat[3][1] * vec[3] + mat[3][2] * vec[3] + mat[3][3] * vec[3]
    );
}

/// @brief Operator* overload for multiplying a Matrix4 by a Matrix4
/// @tparam T Data type of the matrices
/// @param left LHS Matrix
/// @param right RHS Matrix
/// @return The result of the multiplication operation
template <typename T>
Matrix4<T> operator*(const Matrix4<T>& left, const Matrix4<T>& right)
{
    return Matrix4<T>(
        left[0] * right[0][0] + left[0] * right[0][1] + left[0] * right[0][2] + left[0] * right[0][3],
        left[1] * right[1][0] + left[1] * right[1][1] + left[1] * right[1][2] + left[1] * right[1][3],
        left[2] * right[2][0] + left[2] * right[2][1] + left[2] * right[2][2] + left[2] * right[2][3],
        left[3] * right[3][0] + left[3] * right[3][1] + left[3] * right[3][2] + left[3] * right[3][3]
    );
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

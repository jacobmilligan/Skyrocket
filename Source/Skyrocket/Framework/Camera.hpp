//
//  Camera.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 28/10/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include "Skyrocket/Core/Math/Matrix4.hpp"
#include "Skyrocket/Core/Math/Vector3.hpp"

namespace sky {


class Camera3D {
public:
    Camera3D();
    Matrix4f get_matrix();

    void setup(float yfov, float aspect, float near_plane, float far_plane);

    void move(const Vector3f& movement);

    inline void set_position(const Vector3f& pos)
    {
        position_ = pos;
    }

    inline const Vector3f& position() const
    {
        return position_;
    }
private:
    float yfov_, aspect_, near_, far_;
    Vector3f front_, up_;
    Vector3f position_;
    Matrix4f projection_;

    void update_matrix();
};


class Camera2D {
public:
    Camera2D();
    Matrix4f get_matrix();

    void setup(Vector2f view, float near, float far);
    void move(const Vector2f& movement);

    inline void set_position(const Vector2f& pos)
    {
        position_ = Vector3f(-pos, 1.0f);
    }

    inline Vector2f position() const
    {
        return Vector2f(position_.x, position_.y);
    }
private:
    float left_, right_, bottom_, top_, near_, far_;
    Vector3f front_, up_;
    Vector3f position_;
    Matrix4f projection_;
};


} // namespace sky
//
//  Camera3d.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 28/10/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Core/Math/Trigonometry.hpp"
#include "Skyrocket/Framework/Camera.hpp"

namespace sky {


Camera3D::Camera3D()
    : yfov_(static_cast<float>(math::to_radians(90.0f))),
      aspect_(0.0f),
      near_(0.1f),
      far_(5000.0f),
      front_(0.0f, 0.0f, -1.0f),
      up_(0.0f, 1.0f, 0.0f)
{
    update_matrix();
}

void Camera3D::update_matrix()
{
    projection_ = Matrix4f::perspective(yfov_, aspect_, near_, far_);
}

Matrix4f Camera3D::get_matrix()
{
    auto view = Matrix4f::look_at(position_, position_ + front_, up_);
    return projection_ * view;
}

void Camera3D::setup(const float yfov, const float aspect, const float near_plane, const float far_plane)
{
    yfov_ = static_cast<float>(math::to_radians(yfov));
    aspect_ = aspect;
    near_ = near_plane;
    far_ = far_plane;

    update_matrix();
}

void Camera3D::move(const Vector3f& movement)
{
    position_ += movement;
    position_ += front_.cross(up_) * movement;
}

Camera2D::Camera2D()
    : left_(0.0f),
      right_(800.0f),
      bottom_(0.0f),
      top_(0.0f),
      near_(0.0f),
      far_(1.0f),
      position_(0.0f),
      front_(0.0f, 0.0f, 1.0f),
      up_(0.0f, 0.0f, 0.0f)
{
    projection_ = Matrix4f::ortho(left_, right_, bottom_, top_, near_, far_);
}

Matrix4f Camera2D::get_matrix()
{
    return projection_ * Matrix4f::translate(position_);
}

void Camera2D::setup(Vector2f view, const float near, const float far)
{
    right_ = view.x;
    top_ = view.y;
    near_ = near;
    far_ = far;

    projection_ = Matrix4f::ortho(left_, right_, bottom_, top_, near_, far_);
}

void Camera2D::move(const Vector2f& movement)
{
    // Off
    position_.x -= movement.x;
    position_.y += movement.y;
}


} // namespace sky
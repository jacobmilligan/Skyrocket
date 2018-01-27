//
//  Camera.cpp
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


Camera::Camera()
    : yfov_(static_cast<float>(math::to_radians(90.0f))),
      aspect_(0.0f),
      near_(0.1f),
      far_(5000.0f),
      front_(0.0f, 0.0f, -1.0f),
      up_(0.0f, 1.0f, 0.0f),
      I_(1.0f)
{
    update_matrix();
}

void Camera::update_matrix()
{
    perspective_ = Matrix4f::perspective(yfov_, aspect_, near_, far_);
}

Matrix4f Camera::get_matrix()
{
    auto view = Matrix4f::look_at(pos_, pos_ + front_, up_);
    return perspective_ * view;
}

void Camera::setup(const float yfov, const float aspect, const float near_plane, const float far_plane)
{
    yfov_ = static_cast<float>(math::to_radians(yfov));
    aspect_ = aspect;
    near_ = near_plane;
    far_ = far_plane;

    update_matrix();
}

void Camera::set_position(const Vector3f& pos)
{
    pos_ = pos;
}

void Camera::move(const Vector3f& movement)
{
    pos_ += movement;
    pos_ += front_.cross(up_) * movement;
}

Camera2D::Camera2D()
{

}

Matrix4f Camera2D::get_matrix()
{
    return {};
}

void Camera2D::setup()
{

}


} // namespace sky
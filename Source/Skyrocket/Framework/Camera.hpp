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


class Camera {
public:
    Camera();
    Matrix4f get_matrix();

    void setup(float yfov, float aspect, float near_plane, float far_plane);

    void set_position(const Vector3f& pos);
    void move(const Vector3f& movement);
private:
    float yfov_, aspect_, near_, far_;
    Vector3f pos_, front_, up_;
    Matrix4f I_, perspective_;

    void update_matrix();
};

class Camera2D {
public:
    Camera2D();
    Matrix4f get_matrix();

    void setup();
private:
    Matrix4f matrix_;
};


} // namespace sky
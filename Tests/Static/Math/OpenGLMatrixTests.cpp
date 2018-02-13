//
//  OpenGLMatrixTests.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 13/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include <Skyrocket/Core/Math/Matrix4.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "catch/catch.hpp"

glm::highp_vec3 to_glm(const sky::Vector3f& vec)
{
    return glm::vec3(vec.x, vec.y, vec.z);
}

TEST_CASE("Perspective projection is correct", "[gl_matrix]")
{
    sky::Matrix::depth = sky::ClipSpaceDepth::negative_one_to_one;

    auto constexpr fovy = 1.57079637f;
    auto constexpr aspect = 0.0f;
    auto constexpr near = 0.100000001f;
    auto constexpr far = 5000.0f;

    auto glm_persp = glm::perspectiveLH_ZO(fovy, aspect, near, far);
    auto sky_persp = sky::Matrix4f::perspective(fovy, aspect, near, far);

    auto temp = glm::value_ptr(glm_persp);
    for (int i = 0; i < 16; ++i) {
        auto glm_val = temp[i];
        auto sky_val = sky_persp.entries[i];
        REQUIRE(sky_val == Approx(glm_val));
    }
}

TEST_CASE("Look at is correct", "[gl_matrix]")
{
    sky::Matrix::depth = sky::ClipSpaceDepth::negative_one_to_one;

    auto const eye = sky::Vector3f(0.0f, 0.0f, 3.0f);
    auto const target = sky::Vector3f(0.0f, 0.0f, 0.0f);
    auto const up = sky::Vector3f(0.0f, 1.0f, 0.0f);

    auto glm_lookat = glm::lookAtLH(to_glm(eye), to_glm(target), to_glm(up));
    auto sky_lookat = sky::Matrix4f::look_at(eye, target, up);

    auto temp = glm::value_ptr(glm_lookat);
    for (int i = 0; i < 16; ++i) {
        auto glm_val = temp[i];
        auto sky_val = sky_lookat.entries[i];
        REQUIRE(sky_val == Approx(glm_val));
    }
}

TEST_CASE("Orthographic projection is correct", "[gl_matrix]")
{
    sky::Matrix::depth = sky::ClipSpaceDepth::negative_one_to_one;

    auto constexpr left = 0.0f;
    auto constexpr right = 800.0f;
    auto constexpr bottom = 0.0f;
    auto constexpr top = 600.0f;
    auto constexpr near = 0.100000001f;
    auto constexpr far = 1000.0f;

    auto glm_ortho = glm::orthoLH_ZO(left, right, bottom, top, near, far);
    auto sky_ortho = sky::Matrix4f::ortho(left, right, bottom, top, near, far);

    auto temp = glm::value_ptr(glm_ortho);
    for (int i = 0; i < 16; ++i) {
        auto glm_val = temp[i];
        auto sky_val = sky_ortho.entries[i];
        REQUIRE(sky_val == Approx(glm_val));
    }
}

TEST_CASE("Tranformation is correct", "[gl_matrix]")
{
    auto xaxis_ = sky::Vector3f(0.0f, 1.0f, 0.0f);
    auto yaxis_ = sky::Vector3f(1.0f, 0.0f, 0.0f);
    auto pos = sky::Vector3f(0.0f, 0.0f, 0.0f);
    auto scale = sky::Vector3f(100.0f, 100.0f, 100.0f);
    auto angle = sky::Vector2f(0.0f, 1.0f);

    auto translation_mat = sky::Matrix4f::translate(pos);
    auto scale_mat = sky::Matrix4f::scale(scale);
    auto rotation_mat = sky::Matrix4f::rotate(glm::radians(90.0f), xaxis_);
    auto sky_transform = translation_mat * rotation_mat * scale_mat;

    glm::mat4 glmtrans_mat(1.0f), glmscale_mat(1.0f), glmrot_mat(1.0f);
    auto glm_trans = glm::translate(glmtrans_mat, to_glm(pos));
    auto glm_scale = glm::scale(glmscale_mat, to_glm(scale));
    auto glm_rot = glm::rotate(glmrot_mat, glm::radians(90.0f), to_glm(xaxis_));

    auto temp = glm::value_ptr(glm_trans);
    for (int i = 0; i < 16; ++i) {
        auto glm_val = temp[i];
        auto sky_val = translation_mat.entries[i];
        REQUIRE(sky_val == Approx(glm_val));
    }

    temp = glm::value_ptr(glm_scale);
    for (int i = 0; i < 16; ++i) {
        auto glm_val = temp[i];
        auto sky_val = scale_mat.entries[i];
        REQUIRE(sky_val == Approx(glm_val));
    }

    temp = glm::value_ptr(glm_rot);
    for (int i = 0; i < 16; ++i) {
        auto glm_val = temp[i];
        auto sky_val = rotation_mat.entries[i];
        REQUIRE(sky_val == Approx(glm_val));
    }

    auto glm_transform = glm_trans * glm_rot * glm_scale;
    temp = glm::value_ptr(glm_transform);
    for (int i = 0; i < 16; ++i) {
        auto glm_val = temp[i];
        auto sky_val = sky_transform.entries[i];
        REQUIRE(sky_val == Approx(glm_val));
    }
}


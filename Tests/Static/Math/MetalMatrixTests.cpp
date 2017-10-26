//
//  MathTests.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 21/08/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#define SKY_GRAPHICS_API_METAL 1

#include <Skyrocket/Core/Math/Matrix4.hpp>
#include <Skyrocket/Platform/Time.hpp>
#include <Skyrocket/Core/Diagnostics/Timespan.hpp>

#include "MBEMatrixUtilities.h"
#include "catch.hpp"

TEST_CASE("test_speed", "[matrix]")
{
    sky::Matrix4f identity, mat;
    auto pos = sky::Vector3f(10.0f, 10.0f, 10.0f);

    auto start = sky::high_resolution_time();

    for ( int i = 0; i < 50000; ++i ) {
        mat = identity * identity;
    }

    auto end = sky::Timespan(sky::high_resolution_time() - start);

    float m1[16];
    float m2[16];

    for ( int k = 0; k < 16; ++k ) {
        m1[k] = 1;
        m2[k] = 1;
    }

    float result[16];
    start = sky::high_resolution_time();

    for ( int i = 0; i < 50000; ++i ) {
        result[0] = m2[0] * m1[0] + m2[1] * m1[4] + m2[2] * m1[8] + m2[3] * m1[12];
        result[1] = m2[0] * m1[1] + m2[1] * m1[5] + m2[2] * m1[9] + m2[3] * m1[13];
        result[2] = m2[0] * m1[2] + m2[1] * m1[6] + m2[2] * m1[10] + m2[3] * m1[14];
        result[3] = m2[0] * m1[3] + m2[1] * m1[7] + m2[2] * m1[11] + m2[3] * m1[15];

        result[4] = m2[4] * m1[0] + m2[5] * m1[4] + m2[6] * m1[8] + m2[7] * m1[12];
        result[5] = m2[4] * m1[1] + m2[5] * m1[5] + m2[6] * m1[9] + m2[7] * m1[13];
        result[6] = m2[4] * m1[2] + m2[5] * m1[6] + m2[6] * m1[10] + m2[7] * m1[14];
        result[7] = m2[4] * m1[3] + m2[5] * m1[7] + m2[6] * m1[11] + m2[7] * m1[15];

        result[8] =  m2[8] * m1[0] + m2[9] * m1[4] + m2[10] * m1[8] + m2[11] * m1[12];
        result[9] =  m2[8] * m1[1] + m2[9] * m1[5] + m2[10] * m1[9] + m2[11] * m1[13];
        result[10] = m2[8] * m1[2] + m2[9] * m1[6] + m2[10] * m1[10] + m2[11] * m1[14];
        result[11] = m2[8] * m1[3] + m2[9] * m1[7] + m2[10] * m1[11] + m2[11] * m1[15];

        result[12] =  m2[12] * m1[0] + m2[13] * m1[4] + m2[14] * m1[8] + m2[15] * m1[12];
        result[13] =  m2[12] * m1[1] + m2[13] * m1[5] + m2[14] * m1[9] + m2[15] * m1[13];
        result[14] =  m2[12] * m1[2] + m2[13] * m1[6] + m2[14] * m1[10] + m2[15] * m1[14];
        result[15] =  m2[12] * m1[3] + m2[13] * m1[7] + m2[14] * m1[11] + m2[15] * m1[15];
    }

    auto mtl_end = sky::Timespan(sky::high_resolution_time() - start);

    printf("Metal: %f, Skyrocket: %f\n", mtl_end.total_milliseconds(), end.total_milliseconds());

    sky::Vector3f cam_pos(5000.0f, 5000.0f, 2000.0f);
    sky::Vector3f cam_front(0.0f, 0.0f, -1.0f);
    sky::Vector3f cam_up(0.0f, 1.0f, 0.0f);
    auto view_mat = identity.look_at(cam_pos, cam_pos + cam_front, cam_up);

    printf("%s\n", view_mat.to_string().c_str());
}

TEST_CASE("test_projection_matrices", "[matrix]")
{
    SECTION("perspective")
    {
        auto aspect = 800.0f / 600.0f;
        auto sky_perspective = sky::Matrix4f(1.0f).perspective(45.0f, aspect, 0.1f, 100.0f);
        auto mtl_perspective = matrix_perspective_projection(aspect, 45.0f, 0.1f, 100.0f);

        for ( int col = 0; col < 4; col++ ) {
            for ( int row = 0; row < 4; row++ ) {
                INFO("Row: " << row << " Col: " << col);
                REQUIRE(sky_perspective[col][row] == Approx(mtl_perspective.columns[col][row]));
            }
        }
    }
}

TEST_CASE("test_linear_transformations", "[matrix]")
{
    SECTION("translation")
    {
        auto sky_trans = sky::Matrix4f(1.0f).translate(sky::Vector3f(10.0f, 10.0f, 10.0f));
        vector_float3 vec = { 10.0f, 10.0f, 10.0f };
        auto mtl_trans = matrix_translation(vec);

        for ( int col = 0; col < 4; col++ ) {
            for ( int row = 0; row < 4; row++ ) {
                INFO("Row: " << row << " Col: " << col);
                REQUIRE(sky_trans[col][row] == Approx(mtl_trans.columns[col][row]));
            }
        }
    }

    SECTION("scale")
    {
        auto sky_scale = sky::Matrix4f(1.0f).scale(sky::Vector3f(10.0f, 10.0f, 10.0f));
        vector_float3 vec = { 10.0f, 10.0f, 10.0f };
        auto mtl_scale = matrix_scale(vec);

        for ( int col = 0; col < 4; col++ ) {
            for ( int row = 0; row < 4; row++ ) {
                INFO("Row: " << row << " Col: " << col);
                REQUIRE(sky_scale[col][row] == Approx(mtl_scale.columns[col][row]));
            }
        }
    }

    SECTION("rotation")
    {
        auto sky_rot = sky::Matrix4f(1.0f).rotate(45.0f, sky::Vector3f(1.0f, 0.0f, 0.0f));
        vector_float3 vec = { 1.0f, 0.0f, 0.0f };
        auto mtl_rot = matrix_rotation(vec, 45.0f);

        for ( int col = 0; col < 4; col++ ) {
            for ( int row = 0; row < 4; row++ ) {
                INFO("Row: " << row << " Col: " << col);
                REQUIRE(sky_rot[col][row] == Approx(mtl_rot.columns[col][row]));
            }
        }
    }
}

TEST_CASE("test_matrix_multiplication", "[matrix]")
{
    auto compare_matrices = [&](const sky::Matrix4f& sky, const matrix_float4x4& mtl) {
        for ( int col = 0; col < 4; col++ ) {
            for ( int row = 0; row < 4; row++ ) {
                if ( sky[col][row] != Approx(mtl.columns[col][row]) ) {
                    WARN("Row: " << row << " Col: " << col);
                    return false;
                }
            }
        }

        return true;
    };

    auto aspect = 800.0f / 600.0f;
    auto sky_perspective = sky::Matrix4f(1.0f).perspective(45.0f, aspect, 0.1f, 100.0f);
    auto mtl_perspective = matrix_perspective_projection(aspect, 45.0f, 0.1f, 100.0f);

    auto sky_trans = sky::Matrix4f(1.0f).translate(sky::Vector3f(10.0f, 10.0f, 10.0f));
    vector_float3 vec = { 10.0f, 10.0f, 10.0f };
    auto mtl_trans = matrix_translation(vec);
    REQUIRE(compare_matrices(sky_trans, mtl_trans));

    auto sky_scale = sky::Matrix4f(1.0f).scale(sky::Vector3f(10.0f, 10.0f, 10.0f));
    vec = { 10.0f, 10.0f, 10.0f };
    auto mtl_scale = matrix_scale(vec);
    REQUIRE(compare_matrices(sky_scale, mtl_scale));

    auto sky_rot = sky::Matrix4f(1.0f).rotate(45.0f, sky::Vector3f(1.0f, 0.0f, 0.0f));
    vec = { 1.0f, 0.0f, 0.0f };
    auto mtl_rot = matrix_rotation(vec, 45.0f);
    REQUIRE(compare_matrices(sky_rot, mtl_rot));

    // Check each multiplication operation
    auto sky_mvp = sky_rot * sky_scale;
    auto mtl_mvp = matrix_multiply(mtl_rot, mtl_scale);
    REQUIRE(compare_matrices(sky_mvp, mtl_mvp));

    sky_mvp = sky_trans * sky_mvp;
    mtl_mvp = matrix_multiply(mtl_trans, mtl_mvp);
    REQUIRE(compare_matrices(sky_mvp, mtl_mvp));

    sky_mvp = sky_perspective * sky_mvp;
    mtl_mvp = matrix_multiply(mtl_perspective, mtl_mvp);
    REQUIRE(compare_matrices(sky_mvp, mtl_mvp));
}

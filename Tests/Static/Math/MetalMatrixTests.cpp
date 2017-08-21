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

#include "MBEMatrixUtilities.h"
#include "catch.hpp"

#include <simd/simd.h>

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

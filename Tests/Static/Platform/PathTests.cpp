//
//  PathTests.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 7/02/2018
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include <Skyrocket/Platform/Filesystem.hpp>

#include "catch/catch.hpp"

TEST_CASE("Path returns correct executable path", "[path]")
{
    auto exe = sky::Path::executable_path();
//    "/Users/Jacob/Dev/Repos/Skyrocket/Build/Debug/Tests/Static/Platform/PlatformTests"
    auto parent = sky::Path(exe.parent());
    auto is_equal = strcmp(parent.filename(), "Platform") == 0;
    REQUIRE(is_equal);

    parent = sky::Path(parent.parent());
    is_equal = strcmp(parent.filename(), "Static") == 0;
    REQUIRE(is_equal);

    parent = sky::Path(parent.parent());
    is_equal = strcmp(parent.filename(), "Tests") == 0;
    REQUIRE(is_equal);

    parent = sky::Path(parent.parent());
    is_equal = strcmp(parent.filename(), "Debug") == 0;
    REQUIRE(is_equal);

    parent = sky::Path(parent.parent());
    is_equal = strcmp(parent.filename(), "Build") == 0;
    REQUIRE(is_equal);

    parent = sky::Path(parent.parent());
    is_equal = strcmp(parent.filename(), "Skyrocket") == 0;
    REQUIRE(is_equal);
}

TEST_CASE("Appending one path to another returns correct string", "[path]")
{
    sky::Path path("/This/Is/A/Test/Path");
    sky::Path path2("So/Good");
    path.append(path2);
    REQUIRE(strcmp(path.str(), "/This/Is/A/Test/Path/So/Good") == 0);
}

TEST_CASE("Appending a string to another returns correct string", "[path]")
{
    sky::Path path("/This/Is/A/Test/Path");
    path.append("So/Good");
    REQUIRE(strcmp(path.str(), "/This/Is/A/Test/Path/So/Good") == 0);
}

TEST_CASE("Setting extension for path without extension returns correct string", "[path]")
{
    sky::Path path("/This/Is/A/Test/Path");
    path.set_extension("txt");
    REQUIRE(strcmp(path.str(), "/This/Is/A/Test/Path.txt") == 0);
}

TEST_CASE("Setting extension for path with an extension returns correct string", "[path]")
{
    sky::Path path("/This/Is/A/Test/Path.txt");
    path.set_extension("jpg");
    REQUIRE(strcmp(path.str(), "/This/Is/A/Test/Path.jpg") == 0);
}

TEST_CASE("make_real removes symlinks", "[path]")
{
    auto exe_path = sky::Path::executable_path();
    auto test_path = exe_path.relative_path("..").relative_path("..");
    test_path.make_real();
    auto expected_path = sky::Path(exe_path.parent()).parent();

    REQUIRE(strcmp(test_path.str(), expected_path) == 0);
}

TEST_CASE("Path::exists returns true for paths that exist", "[path]")
{
    auto exe_path = sky::Path::executable_path();
    REQUIRE(exe_path.exists());
}

TEST_CASE("Path::exists returns false for paths that don't exist", "[path]")
{
    sky::Path path("/This/Is/A/Test/Path");
    REQUIRE(!path.exists());
}

TEST_CASE("Path returns filename for paths with extensions", "[path]")
{
    sky::Path path("/This/Is/A/Test/Path.txt");
    auto filename = path.filename();
    REQUIRE(strcmp(filename, "Path.txt") == 0);
}

TEST_CASE("Path returns filename for paths without extensions", "[path]")
{
    sky::Path path("/This/Is/A/Test/Path");
    auto filename = path.filename();
    REQUIRE(strcmp(filename, "Path") == 0);
}

TEST_CASE("Path returns correct parent directory", "[path]")
{
    sky::Path path("/This/Is/A/Test/Path");
    auto parent = path.parent();
    REQUIRE(strcmp(parent, "/This/Is/A/Test") == 0);


    sky::Path path2("/Users/Jacob/Dev/Repos/Skyrocket/Build/Debug/Tests/Static/Platform/PlatformTests");
    auto parent2 = path2.parent();
    REQUIRE(strcmp(parent2, "/Users/Jacob/Dev/Repos/Skyrocket/Build/Debug/Tests/Static/Platform") == 0);
}

TEST_CASE("Stem returns just the filename component without extension", "[path]")
{
    sky::Path path("/This/Is/A/Test/Path.txt");
    auto stem = path.stem();
    REQUIRE(strcmp(stem, "Path") == 0);
}

TEST_CASE("Relative path returns correct string", "[path]")
{
    sky::Path path("/This/Is/A/Test/Path");
    auto rel = path.relative_path("This").relative_path("Is").relative_path("Relative");
    REQUIRE(strcmp(rel.str(), "/This/Is/A/Test/Path/This/Is/Relative") == 0);
}

TEST_CASE("Paths report correct size", "[path]")
{
    auto str = "/This/Is/A/Test/Path";
    auto len = strlen(str);
    sky::Path path(str);
    REQUIRE(path.size() == len);

    auto str2 = "/This/Is/A/Test/Path/../../../with/weird/stuff.txt.hey";
    len = strlen(str2);
    path = sky::Path(str2);
    REQUIRE(path.size() == len);
}

TEST_CASE("Path equality works for equal paths", "[path]")
{
    sky::Path path("/This/Is/A/Test/Path");
    sky::Path path2("/This/Is/A/Test/Path");

    REQUIRE(path == path2);
}

TEST_CASE("Path inequality works for unequal paths", "[path]")
{
    sky::Path path("/This/Is/A/Test/Path");
    sky::Path path2("/This/Is/A/Test/Path/../..");

    REQUIRE(path != path2);
}
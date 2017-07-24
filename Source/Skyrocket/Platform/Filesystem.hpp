//
//  Filesystem.hpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 23/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

#include <cstdint>
#include <vector>

namespace sky {


class Path {
public:
    Path();
    explicit Path(const char* path);
    Path(const Path& other);

    static Path executable_path();

    void append(const Path& other);
    void append(const char* str);

    bool exists() const;

    const char* filename() const;
    const char* parent() const;

    const char* str() const;
    uint32_t size() const;

    bool operator==(const Path& other) const;
    bool operator!=(const Path& other) const;
private:
    static const char slash_;

    std::vector<char> path_;

    void make_real();
    void make_null_terminated();
    int32_t last_slash_pos() const;
};


}  // namespace sky
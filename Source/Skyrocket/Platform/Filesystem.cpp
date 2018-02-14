//
//  Filesystem.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 23/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Platform/Filesystem.hpp"
#include "Skyrocket/Core/Diagnostics/Error.hpp"

#include <cstdio>
#include <fstream>
#include <sstream>

namespace sky {


Path::Path()
{
    make_null_terminated();
}

Path::Path(const char* path)
{
    back_ = strlen(path);
    strcpy(path_, path);
    if (exists()) {
        make_real();
    }
    make_null_terminated();
}

Path::Path(const Path& other)
{
    strcpy(path_, other.path_);
    back_ = other.back_;
}

Path& Path::operator=(const Path& other)
{
    strcpy(path_, other.path_);
    back_ = other.back_;
    return *this;
}

void Path::set_extension(const char* ext)
{
    auto last_dot = last_char_pos('.');
    auto len = strlen(ext);
    if (last_dot == -1) {
        // no extension - append one
        path_[back_++] = '.';
        last_dot = last_char_pos('.');
    }

    strncpy(path_ + last_dot + 1, ext, len);
    back_ += len;
    make_null_terminated();
}

void Path::append(const Path& other)
{
    append(other.path_);
}

void Path::append(const char* str)
{
    auto last_slash = last_slash_pos();
    auto len = strlen(str);

    if (last_slash != back_) {
        path_[back_++] = slash_;
        last_slash = last_slash_pos();
    }

    strncpy(path_ + last_slash + 1, str, len);
    back_ += len;

    make_null_terminated();
    if ( exists() ) {
        make_real();
        make_null_terminated();
    }
}

const char* Path::filename() const
{
    static char temp[SKY_MAX_PATH];

    auto last_slash = last_slash_pos();
    if ( last_slash == -1 ) {
        return path_;
    }

    strncpy(temp, path_ + last_slash + 1, back_);
    temp[back_] = '\0';
    return temp;
}

const char* Path::stem() const
{
    static char temp[SKY_MAX_PATH];

    auto last_dot = last_char_pos('.');
    if ( last_dot == -1 ) {
        return path_;
    }

    auto last_slash = last_slash_pos();
    if ( last_slash == -1 ) {
        return path_;
    }

    strncpy(temp, path_ + last_slash + 1, static_cast<size_t>(last_dot - last_slash - 1));
    return temp;
}

Path Path::parent() const
{
    static char temp[SKY_MAX_PATH];

    auto last_slash = last_slash_pos();
    if ( last_slash == -1 ) {
        return *this;
    }

    strncpy(temp, path_, static_cast<size_t>(last_slash));
    temp[last_slash] = '\0';
    return Path(temp);
}

bool Path::operator==(const Path& other) const
{
    if (back_ != other.back_) {
        return false;
    }

    for ( int i = 0; i < back_; ++i ) {
        if ( path_[i] != other.path_[i] ) {
            return false;
        }
    }

    return true;
}

bool Path::operator!=(const Path& other) const
{
    return !(*this == other);
}

const char* Path::str() const
{
    return path_;
}

Path Path::relative_path(const char* str) const
{
    auto rel = *this;
    rel.append(str);
    return rel;
}

uint32_t Path::size() const
{
    return static_cast<uint32_t>(back_);
}

int32_t Path::last_char_pos(const char c) const
{
    auto size = static_cast<int32_t>(back_) - 1;
    for ( int32_t i = size; i > 0; --i ) {
        if ( path_[i] == c ) {
            return i;
        }
    }

    return -1;
}

int32_t Path::last_slash_pos() const
{
    return last_char_pos(slash_);
}

void Path::make_null_terminated()
{
    if ( back_ <= 0 || path_[back_] != '\0' ) {
        path_[back_] = '\0';
    }
}


namespace fs {


std::string slurp_file(const Path& path)
{
    std::ifstream file(path.str());

    if ( !file ) {
        SKY_ERROR("Slurping file", "No such file found with the specified name %s", path.str());
        return "";
    }

    std::stringstream ss;
    ss << file.rdbuf();
    file.close();

    return ss.str();
}


} // namespace fs


} // namespace sky
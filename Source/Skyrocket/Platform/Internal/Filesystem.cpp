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

#include <cstdio>
#include <cstring>

namespace sky {


Path::Path()
{
    make_real();
}

Path::Path(const char* path)
{
    path_ = std::vector<char>(path, path + strlen(path));
    make_null_terminated();
    if ( exists() ) {
        make_real();
    }
}

Path::Path(const Path& other)
    : path_(other.path_)
{
    if ( exists() ) {
        make_real();
    }
}

void Path::append(const Path& other)
{
    append(other.path_.data());
}

void Path::append(const char* str)
{
    auto last_slash = last_slash_pos();

    if ( path_.back() == '\0' ) {
        path_.pop_back();
    }
    if ( last_slash != path_.size() - 1 ) {
        path_.push_back(slash_);
    }

    path_.insert(path_.end(), str, &str[strlen(str)]);

    make_null_terminated();
}

const char* Path::filename() const
{
    auto last_slash = last_slash_pos();
    if ( last_slash == -1 ) {
        return path_.data();
    }

    return std::vector<char>(path_.begin() + last_slash + 1, path_.end()).data();
}

const char* Path::parent() const
{
    auto last_slash = last_slash_pos();
    if ( last_slash == -1 ) {
        return path_.data();
    }

    return std::vector<char>(path_.begin(), path_.begin() + last_slash).data();
}

bool Path::operator==(const Path& other) const
{
    for ( int i = 0; i < path_.size(); ++i ) {
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
    return path_.data();
}

uint32_t Path::size() const
{
    return static_cast<uint32_t>(path_.size());
}

int32_t Path::last_slash_pos() const
{
    auto size = static_cast<int32_t>(path_.size()) - 1;
    for ( int32_t i = size; i > 0; --i ) {
        if ( path_[i] == slash_ ) {
            return i;
        }
    }

    return -1;
}

void Path::make_null_terminated()
{
    if ( path_.back() != '\0' ) {
        path_.push_back('\0');
    }
}


} // namespace sky
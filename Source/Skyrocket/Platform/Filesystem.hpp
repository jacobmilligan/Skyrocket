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


#define SKY_MAX_PATH 256


/// @brief The path class is a lightweight path utility class for navigating the platforms
/// filesystem
class Path {
public:
    Path();
    explicit Path(const char* path);
    Path(const Path& other);

    Path& operator=(const Path& other);

    /// @brief Gets the absolute path to the application binary's directory
    /// @return
    static Path executable_path();

    /// @brief Appends another path to this one
    /// @param other
    void append(const Path& other);
    /// @brief Appends a string to the end of this path
    /// @param str
    void append(const char* str);

    void set_extension(const char* ext);

    /// @brief Removes symlinks etc. from this path
    void make_real();

    /// @brief Checks whether or not this path exists within the filesystem
    /// @return
    bool exists() const;

    /// @brief Gets the filename component of the path, i.e. given `/usr/local/bin/ls` this
    /// function would return `ls`
    /// @return
    const char* filename() const;
    /// @brief Gets the absolute parent directory of the filename component, i.e. given
    /// `/usr/local/bin/ls` this function would return `bin`
    /// @return
    const char* parent() const;

    /// @brief Gets the filename component of the path without the extension, i.e. given `File.txt`
    /// this function would return `File`
    /// @return
    const char* stem() const;

    /// @brief Gets a modified version of this path with a new string appended to the end
    /// @param str
    /// @return
    Path relative_path(const char* str);

    /// @brief Gets the raw string representation of this path
    /// @return
    const char* str() const;

    /// @brief Gets the character size of the path string
    /// @return
    uint32_t size() const;

    inline const char* begin() const
    {
        return &path_[0];
    }

    inline const char* end() const
    {
        return &path_[SKY_MAX_PATH];
    }

    bool operator==(const Path& other) const;
    bool operator!=(const Path& other) const;
private:
    static const char slash_;

    size_t back_;
    char path_[SKY_MAX_PATH];

    void make_null_terminated();
    int32_t last_slash_pos() const;
    int32_t last_char_pos(const char c) const;
};

namespace fs {


std::string slurp_file(const Path& filepath);


} // namespace fs


}  // namespace sky
//
//  MacFilesystem.cpp
//  Skyrocket
//
//  --------------------------------------------------------------
//
//  Created by
//  Jacob Milligan on 23/07/2017
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#include "Skyrocket/Core/Diagnostics/Error.hpp"
#include "Skyrocket/Platform/Filesystem.hpp"

#include <cerrno>
#include <mach-o/dyld.h>
#include <sys/stat.h>

namespace sky {


const char Path::slash_ = '/';

Path Path::executable_path()
{
    char buf[UINT16_MAX];
    uint32_t size = sizeof(buf);
    auto success = _NSGetExecutablePath(buf, &size);

    if ( success != 0 ) {
        SKY_ERROR("Path", "Couldn't get the path to the apps executable directory");
        return Path();
    }

    Path path(buf);

    return path;
}

void Path::make_real()
{
    auto ptr = realpath(path_, nullptr);

    if ( ptr == nullptr ) {
        return;
    }

    strcpy(path_, ptr);

    free(ptr);
}

bool Path::exists() const
{
    struct stat st{};
    return stat(path_, &st) == 0;
}


}  // namespace sky
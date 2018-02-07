set(CMAKE_CXX_STANDARD 14)

set(lib_name SkyrocketCore)

skyrocket_add_sources(Diagnostics/Error.cpp
        Diagnostics/Timespan.cpp
        Hash.cpp
        Math/Math.cpp
        Memory/Memory.cpp)

skyrocket_add_library(${lib_name} STATIC)

skyrocket_configure_platform(compile_flags)
target_compile_options(${lib_name} PRIVATE ${compile_flags})
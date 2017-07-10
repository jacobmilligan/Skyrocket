set(CMAKE_CXX_STANDARD 14)

set(lib_name SkyrocketCore)

skyrocket_add_sources(Diagnostics/Internal/Error.cpp)

skyrocket_add_library(${lib_name} STATIC)
target_link_libraries(${lib_name} SkyrocketPlatform)

skyrocket_configure_platform(compile_flags)
target_compile_options(${lib_name} PRIVATE ${compile_flags})
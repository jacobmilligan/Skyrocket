set(CMAKE_CXX_STANDARD 14)

skyrocket_add_library(SkyrocketIO STATIC)
target_link_libraries(SkyrocketIO SkyrocketCore)

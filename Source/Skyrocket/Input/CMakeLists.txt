set(CMAKE_CXX_STANDARD 14)

add_subdirectory(Internal)

skyrocket_add_library(SkyrocketInput STATIC)
target_link_libraries(SkyrocketInput SkyrocketCore SkyrocketPlatform)


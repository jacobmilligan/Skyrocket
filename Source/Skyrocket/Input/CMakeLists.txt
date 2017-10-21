set(CMAKE_CXX_STANDARD 14)

skyrocket_add_sources(Keyboard.cpp)

skyrocket_add_library(SkyrocketInput STATIC)
target_link_libraries(SkyrocketInput SkyrocketCore SkyrocketPlatform)


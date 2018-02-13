if (APPLE)
    skyrocket_add_test(MetalMatrixTests
            MetalMatrixTests.cpp
            MBEMatrixUtilities.cpp)

    FIND_LIBRARY(metalkit MetalKit)

    if (metalkit)
        MESSAGE("Found MetalKit")
        set(lib "${metal_lib}")
        target_link_libraries(MetalMatrixTests ${metalkit})
    endif()
endif ()

skyrocket_add_test(OpenGLMatrixTests OpenGLMatrixTests.cpp)
target_include_directories(OpenGLMatrixTests PUBLIC ${skyrocket_tests_root}/Static/glm)
if (APPLE)
    skyrocket_add_test(MetalMatrixTests
            Math/MetalMatrixTests.cpp
            Math/MBEMatrixUtilities.cpp)

    FIND_LIBRARY(metalkit MetalKit)

    if (metalkit)
        MESSAGE("Found MetalKit")
        set(lib "${metal_lib}")
        target_link_libraries(MetalMatrixTests ${metalkit})
    endif()
endif ()

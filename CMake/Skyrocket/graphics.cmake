function(skyrocket_configure_graphics_api lib_var)
    set(lib)
    set(api_list)

    if (APPLE)

        FIND_LIBRARY(metal_lib Metal)
        FIND_LIBRARY(opengl_lib OpenGL)

        if (metal_lib)
            list(APPEND lib "${metal_lib}")
            list(APPEND api_list Metal)
            skyrocket_add_definitions(-DSKY_GRAPHICS_API_METAL=1)
        endif()

        if(opengl_lib)
            list(APPEND lib "${opengl_lib}")
            list(APPEND api_list OpenGL)
            skyrocket_add_definitions(-DSKY_GRAPHICS_API_OPENGL=1)
        endif ()

    endif ()
    set(${lib_var} ${lib} PARENT_SCOPE)
    set(skyrocket_graphics_api ${api_list} CACHE INTERNAL "")
endfunction()
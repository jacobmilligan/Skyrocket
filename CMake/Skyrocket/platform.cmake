function(skyrocket_configure_platform compile_flags)
    set(flags)
    if (APPLE)
        set(skyrocket_platform "macOS" CACHE INTERNAL "")
        # Treat all files as objective-c++
        set(flags ${flags} "-xobjective-c++")
    elseif(WIN32)
        set(skyrocket_platform "Windows" CACHE INTERNAL "")
    endif ()

    set(${compile_flags} ${flags} PARENT_SCOPE)
endfunction()
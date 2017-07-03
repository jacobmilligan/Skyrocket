# Adds source files
function(skyrocket_add_sources)
    set(srcs)
    foreach(s IN LISTS ARGN)
        if (NOT IS_ABSOLUTE "${s}")
            get_filename_component(s "${s}" ABSOLUTE)
        endif ()
        list(APPEND srcs "${s}")
    endforeach()

    set(skyrocket_sources ${skyrocket_sources} "${srcs}" CACHE INTERNAL "")
endfunction()

function(skyrocket_add_library lib_name lib_type)
    add_library(${lib_name} "${lib_type}" "${skyrocket_sources}")
    target_include_directories(${lib_name} PUBLIC ${PROJECT_SOURCE_DIR}/Source)
    set(${lib_name}_sources "${skyrocket_sources}" CACHE INTERNAL "")
    unset(skyrocket_sources CACHE)
endfunction()

function(skyrocket_new_module module_name)
    set(${module_name}_sources "${skyrocket_sources}" CACHE INTERNAL "")
    unset(skyrocket_sources CACHE)
endfunction()

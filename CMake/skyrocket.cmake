include(${PROJECT_SOURCE_DIR}/CMake/Skyrocket/platform.cmake)
include(${PROJECT_SOURCE_DIR}/CMake/Skyrocket/graphics.cmake)

# Initializes all cmake functions required by skyrocket
function(skyrocket_init)
    unset(skyrocket_sources CACHE)
    unset(skyrocket_libraries CACHE)
    unset(skyrocket_definitions CACHE)
    unset(skyrocket_build_type CACHE)
    unset(skyrocket_graphics_api CACHE)

    set(skyrocket_build_type DEBUG CACHE INTERNAL "")

    if(CMAKE_CONFIGURATION_TYPES)
        # Multi-config
        foreach(config ${CMAKE_CONFIGURATION_TYPES})
            if(config STREQUAL "Release")
                set(skyrocket_build_type RELEASE)
            endif()
        endforeach()
    else()
        if (CMAKE_BUILD_TYPE STREQUAL "Release")
            set(skyrocket_build_type RELEASE)
        endif ()
    endif()

    skyrocket_add_definitions(-DSKY_${skyrocket_build_type})
endfunction()

# Adds source files to the projects global list
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

# Adds all sources gathered in child directories to a library target
function(skyrocket_add_library lib_name lib_type)
    if (${CMAKE_GENERATOR} STREQUAL "Xcode")
        file(GLOB_RECURSE headers ${CMAKE_CURRENT_SOURCE_DIR}/*.h
                ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
                ${CMAKE_CURRENT_SOURCE_DIR}/*.inl)
    endif ()

    add_library(${lib_name} ${lib_type} "${skyrocket_sources}" "${headers}")
    target_include_directories(${lib_name} PUBLIC ${PROJECT_SOURCE_DIR}/Source)
    target_include_directories(${lib_name} PUBLIC ${PROJECT_SOURCE_DIR}/Deps)
    set(${lib_name}_sources "${skyrocket_sources}" CACHE INTERNAL "")

    set(skyrocket_libraries ${skyrocket_libraries} ${lib_name} CACHE INTERNAL "")
    skyrocket_include_all_definitions()
    unset(skyrocket_sources CACHE)

#    skyrocket_configure_platform(compile_flags)
#    target_compile_options(${lib_name} PRIVATE "${compile_flags}")
endfunction()

# Creates a new list of sources to attach to a specific skyrocket module - doesn't necessarily have
# to be associated with a library - could be a component etc.
function(skyrocket_new_module module_name)
    set(${module_name}_sources "${skyrocket_sources}" CACHE INTERNAL "")
    unset(skyrocket_sources CACHE)
endfunction()

# Adds definitions to the global definitions list for the project
function(skyrocket_add_definitions)
    foreach (def IN LISTS ARGN)
        add_definitions(${def})
        set(skyrocket_definitions ${skyrocket_definitions} "${def}" CACHE INTERNAL "")
    endforeach ()
endfunction()

# Includes all previously added definitions to the entire project
function(skyrocket_include_all_definitions)
    foreach(def IN LISTS skyrocket_definitions)
        add_definitions("${def}")
    endforeach()
endfunction()


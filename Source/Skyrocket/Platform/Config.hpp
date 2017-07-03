//
//  Config.hpp
//  Skyrocket Framework
//
//  --------------------------------------------------------------
//
//  Created by Jacob Milligan on 30/12/2016.
//  Copyright (c) 2016 Jacob Milligan. All rights reserved.
//

#pragma once

namespace sky {

/// @defgroup platform Platform module

/// @defgroup os OS submodule
/// @brief Defines several OS-related macros and constants that can be used
/// for platform-specific code.
///
/// Both the \link target_platform \endlink and \link target_family \endlink
/// constants can and should be used for platform-detection over the predefines
/// macros to avoid horrible macro debugging problems that can crop up.
/// The macros should only be used to conditionally include headers or to
/// conditionally include blocks of code that otherwise wouldn't compile on
/// the current platform. Each OS-detection macro is a boolean value of either 1 or 0
/// @ingroup platform

/// @var target_platform
/// The operating system currently being compiled on
/// @var target_family
/// The family of operating systems currently being compiled on

/// @addtogroup os
/// @{

/// @brief Defines the current operating system
enum class OS {
    unknown,
    mac,
    windows,
    linux,
    ios,
    iphone_sim,
    android
};

/// @brief Defines the family of operating systems that the current platform
/// belongs to
enum class OSFamily {
    unknown,
    apple,
    microsoft,
    linux,
    android
};

/// @brief MacOSX operating system
#define SKYROCKET_OS_MAC_OSX 0

/// @brief IOS operating system
#define SKYROCKET_OS_IOS 0

/// @brief Android operating system
#define SKYROCKET_OS_ANDROID 0

/// @brief Windows operating system
#define SKYROCKET_OS_WINDOWS 0

/// @brief Linux operating systems
#define SKYROCKET_OS_LINUX 0

/// @}


#if defined(__APPLE__) && defined(__MACH__)
const auto target_family = OSFamily::apple;
    #include <TargetConditionals.h>
    
    #if TARGET_IPHONE_SIMULATOR == 1
#define SKYROCKET_OS_IOS
        #define SKYROCKET_OS_IOS 1
        const auto target_platform = OS::iphone_sim;
    
    #elif TARGET_OS_IPHONE == 1
#undef SKYROCKET_OS_IOS
        #define SKYROCKET_OS_IOS 1
        const auto target_platform = OS::ios;
    
    #elif TARGET_OS_MAC == 1
        #undef SKYROCKET_OS_MAC_OSX
        #define SKYROCKET_OS_MAC_OSX 1
const auto target_platform = OS::mac;
    
    #endif
#elif defined(__WIN32__) || defined(__WINDOWS__) || defined(_WIN64) \
|| defined(_WIN32) || defined(_WINDOWS) || defined(__TOS_WIN__)
	#undef SKYROCKET_OS_WINDOWS
    #define SKYROCKET_OS_WINDOWS 1
    const auto target_family = OSFamily::microsoft;
    const auto target_platform = OS::windows;

#elif defined(__linux__) || defined(__linux) || defined(linux)
    #undef SKYROCKET_OS_LINUX
    #define SKYROCKET_OS_LINUX 1
    const auto target_family = OSFamily::linux;
    const auto target_platform = OS::linux;

#elif defined(__ANDROID__)
    #undef SKYROCKET_OS_ANDROID
    #define SKYROCKET_OS_ANDROID 1
    const auto target_family = OSFamily::android;
    const auto target_platform = OS::android;
    const auto android_api_level = __ANDROID_API__;
    
#else
    const auto target_family = OSFamily::unknown;
    const auto target_platform = OS::unknown;
#endif

////////////////////////
// Compiler detection //
////////////////////////

enum class Compiler {
    unknown,
    clang,
    gcc,
    msvc
};

#define SKYROCKET_COMPILER_CLANG 0
#define SKYROCKET_COMPILER_GCC 0
#define SKYROCKET_COMPILER_MSVC 0

#if defined(__clang__)
    #undef SKYROCKET_COMPILER_CLANG
    #define SKYROCKET_COMPILER_CLANG 1
const auto target_compiler = Compiler::clang;
#elif defined(__GNUC__)
#undef SKYROCKET_COMPILER_GCC
    #define SKYROCKET_COMPILER_GCC 1
    const auto target_compiler = Compiler::gcc;
#elif defined(_MSC_VER)
    #undef SKYROCKET_COMPILER_MSVC
    #define SKYROCKET_COMPILER_MSVC 1
    const auto target_compiler = Compiler::msvc;
#else
    const auto target_compiler = Compiler::unknown;
#endif

//////////////////////////////
// Compiler implementations //
//////////////////////////////

#define SKYROCKET_STRINGIFY(x) #x

#if SKYROCKET_COMPILER_CLANG == 1
    #define SKYROCKET_PUSH_WARNING _Pragma("clang diagnostic push")
    #define SKYROCKET_IGNORE_WARNING(x) _Pragma( SKYROCKET_STRINGIFY(clang diagnostic ignored #x) )
    #define SKYROCKET_POP_WARNING _Pragma("clang diagnostic pop")
    #define SKYROCKET_PACKED(n) __attribute__((packed, aligned(n)))
    #define SKYROCKET_FUNCTION_NAME __PRETTY_FUNCTION__
#elif SKYROCKET_COMPILER_GCC == 1
    #define SKYROCKET_PUSH_WARNING _Pragma( SKYROCKET_STRINGIFY(GCC diagnostic push)
    #define SKYROCKET_IGNORE_WARNING(x) _Pragma( SKYROCKET_STRINGIFY(GCC diagnostic ignored #x)
    #define SKYROCKET_POP_WARNING _Pragma( SKYROCKET_STRINGIFY(GCC diagnostic pop)
    #define SKYROCKET_PACKED(n) __attribute__((packed, aligned(n)))
	#define SKYROCKET_FUNCTION_NAME __PRETTY_FUNCTION__
#elif SKYROCKET_COMPILER_MSVC == 1
    #define SKYROCKET_PUSH_WARNING _Pragma( SKYROCKET_STRINGIFY(warning( push ))
    #define SKYROCKET_IGNORE_WARNING(x) _Pragma( SKYROCKET_STRINGIFY(warning( disable: #x ))
    #define SKYROCKET_POP_WARNING _Pragma( SKYROCKET_STRINGIFY(warning( pop ))
	#define SKYROCKET_FUNCTION_NAME __FUNCTION__
#endif



}

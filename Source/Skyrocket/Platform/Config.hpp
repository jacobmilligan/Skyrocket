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
    macos,
    windows,
    linux_generic,
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
    linuxos,
    android
};

/// @brief MacOSX operating system
#define SKY_OS_MACOS 0

/// @brief IOS operating system
#define SKY_OS_IOS 0

/// @brief Android operating system
#define SKY_OS_ANDROID 0

/// @brief Windows operating system
#define SKY_OS_WINDOWS 0

/// @brief Linux operating systems
#define SKY_OS_LINUX 0

/// @}


#if defined(__APPLE__) && defined(__MACH__)
const auto target_family = OSFamily::apple;
    #include <TargetConditionals.h>
    
    #if TARGET_IPHONE_SIMULATOR == 1
        #undef SKY_OS_IOS
        #define SKY_OS_IOS 1
        const auto target_platform = OS::iphone_sim;
    
    #elif TARGET_OS_IPHONE == 1
        #undef SKY_OS_IOS
        #define SKY_OS_IOS 1
        const auto target_platform = OS::ios;
    
    #elif TARGET_OS_MAC == 1
        #undef SKY_OS_MACOS
        #define SKY_OS_MACOS 1
        const auto target_platform = OS::macos;
    
    #endif
#elif defined(__WIN32__) || defined(__WINDOWS__) || defined(_WIN64) \
|| defined(_WIN32) || defined(_WINDOWS) || defined(__TOS_WIN__)
	#undef SKY_OS_WINDOWS
    #define SKY_OS_WINDOWS 1
    const auto target_family = OSFamily::microsoft;
    const auto target_platform = OS::windows;

#elif defined(__linux__) || defined(__linux) || defined(linux_generic)
    #undef SKY_OS_LINUX
    #define SKY_OS_LINUX 1
    const auto target_family = OSFamily::linux_generic;
    const auto target_platform = OS::linux_generic;

#elif defined(__ANDROID__)
    #undef SKY_OS_ANDROID
    #define SKY_OS_ANDROID 1
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

#define SKY_COMPILER_CLANG 0
#define SKY_COMPILER_GCC 0
#define SKY_COMPILER_MSVC 0

#if defined(__clang__)
    #undef SKY_COMPILER_CLANG
    #define SKY_COMPILER_CLANG 1
const auto target_compiler = Compiler::clang;
#elif defined(__GNUC__)
    #undef SKY_COMPILER_GCC
    #define SKY_COMPILER_GCC 1
    const auto target_compiler = Compiler::gcc;
#elif defined(_MSC_VER)
    #undef SKY_COMPILER_MSVC
    #define SKY_COMPILER_MSVC 1
    const auto target_compiler = Compiler::msvc;
#else
    const auto target_compiler = Compiler::unknown;
#endif

//////////////////////////////
// Compiler implementations //
//////////////////////////////

#define SKY_STRINGIFY(x) #x

#if SKY_COMPILER_CLANG == 1
    #define SKY_PUSH_WARNING _Pragma("clang diagnostic push")
    #define SKY_IGNORE_WARNING(x) _Pragma( SKY_STRINGIFY(clang diagnostic ignored #x) )
    #define SKY_POP_WARNING _Pragma("clang diagnostic pop")
    #define SKY_PACKED(n) __attribute__((packed, aligned(n)))
    #define SKY_FUNCTION_NAME __PRETTY_FUNCTION__

    #define SKY_DEBUG_BREAK() asm("int $3")
#elif SKY_COMPILER_GCC == 1
    #define SKY_PUSH_WARNING _Pragma( SKY_STRINGIFY(GCC diagnostic push)
    #define SKY_IGNORE_WARNING(x) _Pragma( SKY_STRINGIFY(GCC diagnostic ignored #x)
    #define SKY_POP_WARNING _Pragma( SKY_STRINGIFY(GCC diagnostic pop)
    #define SKY_PACKED(n) __attribute__((packed, aligned(n)))
	#define SKY_FUNCTION_NAME __PRETTY_FUNCTION__

    #define SKY_DEBUG_BREAK() asm("int $3")
#elif SKY_COMPILER_MSVC == 1
    #define SKY_PUSH_WARNING _Pragma( SKY_STRINGIFY(warning( push ))
    #define SKY_IGNORE_WARNING(x) _Pragma( SKY_STRINGIFY(warning( disable: #x ))
    #define SKY_POP_WARNING _Pragma( SKY_STRINGIFY(warning( pop ))
	#define SKY_FUNCTION_NAME __FUNCTION__

    #define SKY_DEBUG_BREAK() __asm { int 3 }
#endif

///////////////////////////////
// Graphics API definitions  //
///////////////////////////////

#if !defined(SKY_GRAPHICS_API_METAL)
    #define SKY_GRAPHICS_API_METAL 0
#endif
#if !defined(SKY_GRAPHICS_API_OPENGL)
    #define SKY_GRAPHICS_API_OPENGL 0
#endif


}

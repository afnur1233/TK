#pragma once

#ifndef __STDC_HOSTED__
#   error "Not a hosted implementation"
#endif

#ifdef _WIN32
#   ifndef _CRT_SECURE_NO_WARNINGS
#      define _CRT_SECURE_NO_WARNINGS (1)
#   endif
#endif

#ifndef __STDC_LIMIT_MACROS
#    define __STDC_LIMIT_MACROS (1)
#endif

#ifndef __STDC_CONSTANT_MACROS
#    define __STDC_CONSTANT_MACROS (1)
#endif



#ifdef _MSC_VER
#   include <malloc.h>
#   define alloca _alloca
#else
#   include <alloca.h>
#endif

#include <stdalign.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <tgmath.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>



#ifndef uint
#   define uint unsigned int
#endif



#ifndef TK_has_builtin
#   ifdef __has_builtin
#       define TK_has_builtin(...) __has_builtin(__VA_ARGS__)
#   else
#       define TK_has_builtin(...) 0
#   endif
#endif



#ifndef TK_has_extension
#   ifdef __has_extension
#       define TK_has_extension(...) __has_extension(__VA_ARGS__)
#   else
#       define TK_has_extension(...) 0
#   endif
#endif



#ifndef TK_has_include
#   ifdef __has_include
#       define TK_has_include(...) __has_include(__VA_ARGS__)
#   else
#       define TK_has_include(...) 0
#   endif
#endif



#ifndef TK_has_standard_c89
#   if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199409L)
#       define TK_has_standard_c89() 1
#   else
#       define TK_has_standard_c89() 0
#   endif
#endif



#ifndef TK_has_standard_c99
#   if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#       define TK_has_standard_c99() 1
#   else
#       define TK_has_standard_c99() 0
#   endif
#endif



#ifndef TK_has_standard_c11
#   if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
#       define TK_has_standard_c11() 1
#   else
#       define TK_has_standard_c11() 0
#   endif
#endif



#ifndef TK_has_standard_c17
#   if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201710L)
#       define TK_has_standard_c17() 1
#   else
#       define TK_has_standard_c17() 0
#   endif
#endif



#ifndef TK_has_standard_c23
#   if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202311L)
#       define TK_has_standard_c23() 1
#   else
#       define TK_has_standard_c23() 0
#   endif
#endif



#ifndef TK_has_standard_c2y
#   if defined(__STDC_VERSION__) && (__STDC_VERSION__ > 202311L)
#       define TK_has_standard_c2y() 1
#   else
#       define TK_has_standard_c2y() 0
#   endif
#endif



#ifndef TK_has_standard_attributes
#   if defined(__cplusplus) && (__cplusplus >= 201402L)
#       define TK_has_standard_attributes() 1
#   elif TK_has_standard_c23()
#       define TK_has_standard_attributes() 1
#   else
#       define TK_has_standard_attributes() 0
#   endif
#endif



#ifndef __bool_true_false_are_defined
#   ifdef __cplusplus
#   elif TK_has_standard_c23()
#   else
#       define bool _Bool
#       define true ((_Bool)1)
#       define false ((_Bool)0)
#   endif
#endif



#ifndef restrict
#   if TK_has_standard_c99()
#       define restrict restrict
#   elif defined(__cplusplus)
#       define restrict
#   elif defined(_MSC_VER) || defined(__GNUC__) || defined(__clang__)
#       define restrict __restrict
#   else
#       define restrict
#   endif
#endif



#ifndef cast
#   define cast(...) (__typeof__(__VA_ARGS__))
#endif



#ifndef ptr_cast
#   define ptr_cast(...) (__typeof__(__VA_ARGS__))(void *)
#endif



#ifndef TKDEF
#   define TKDEF
#endif



#ifndef TK_discard
#   define TK_discard(...) __VA_OPT__((void)__VA_ARGS__)
#endif



#ifndef TK_deprecated
#   if TK_has_standard_attributes()
#       define TK_deprecated(...) [[deprecated __VA_OPT__((__VA_ARGS__))]]
#   elif defined(__GNUC__) || defined(__clang__)
#       define TK_deprecated(...) __attribute__((deprecated __VA_OPT__((__VA_ARGS__))))
#   elif defined(_MSC_VER)
#       define TK_deprecated(...) __declspec(deprecated __VA_OPT__((__VA_ARGS__)))
#   else
#       define TK_deprecated(...)
#   endif
#endif



#ifndef TK_fallthrough
#   if TK_has_standard_attributes()
#       define TK_fallthrough(...) [[fallthrough]]
#   elif defined(__GNUC__) || defined(__clang__)
#       define TK_fallthrough(...) __attribute__((__fallthrough__))
#   else
#       define TK_fallthrough(...)
#   endif
#endif



#ifndef TK_nodiscard
#   if TK_has_standard_attributes()
#       define TK_nodiscard(...) [[nodiscard __VA_OPT__((__VA_ARGS__))]]
#   elif defined(__GNUC__) || defined(__clang__)
#       define TK_nodiscard(...) __attribute__((warn_unused_result))
#   elif defined(_MSC_VER)
#       define TK_nodiscard(...) _Check_return_
#   else
#       define TK_nodiscard(...)
#   endif
#endif



#ifndef TK_maybe_unused
#   if TK_has_standard_attributes()
#       define TK_maybe_unused() [[maybe_unused]]
#   elif defined(__GNUC__) || defined(__clang__)
#       define TK_maybe_unused() __attribute__((unused))
#   else
#       define TK_maybe_unused()
#   endif
#endif



#ifndef TK_noreturn
#   if TK_has_standard_attributes()
#       define TK_noreturn() [[noreturn]]
#   elif defined(__GNUC__) || defined(__clang__)
#       define TK_noreturn() __attribute__((noreturn))
#   elif defined(_MSC_VER)
#       define TK_noreturn() __declspec(noreturn)
#   else
#       define TK_noreturn()
#   endif
#endif



#ifndef TK_pure
#   if TK_has_standard_attributes() && !defined(__clang__)
#       define TK_pure() [[reproducible]]
#   else
#       define TK_pure()
#   endif
#endif



#ifndef TK_unsequenced_pure
#   if TK_has_standard_attributes() && !defined(__clang__)
#       define TK_unsequenced_pure() [[unsequenced]]
#   else
#       define TK_unsequenced_pure()
#   endif
#endif



#ifndef TK_force_inline
#   if defined(__GNUC__) || defined(__clang__)
#       define TK_force_inline() __attribute__((always_inline)) static inline
#   elif defined(_MSC_VER)
#       define TK_force_inline() __forceinline
#   else
#       define TK_force_inline() static inline
#   endif
#endif



#ifndef TK_unreachable
#   if TK_has_builtin(__builtin_unreachable)
#       define TK_unreachable() __builtin_unreachable()
#   else
#       define TK_unreachable()
#   endif
#endif


#ifndef TK_cliteral
#   ifdef __cplusplus
#       define TK_cliteral(...) __VA_OPT__(__typeof__(__VA_ARGS__))
#   else
#       define TK_cliteral(...) __VA_OPT__((__typeof__(__VA_ARGS__)))
#   endif
#endif



#ifndef TK_cliteral_zero
#   if defined(__cplusplus) || TK_has_standard_c23()
#       define TK_cliteral_zero(...) TK_cliteral(__VA_ARGS__){}
#   else
#       define TK_cliteral_zero(...) TK_cliteral(__VA_ARGS__){0}
#   endif
#endif



#ifndef TK_arraylength
#   if TK_has_extension(c_countof)
#       define TK_arraylength(...) _Countof(__typeof__(__VA_ARGS__))
#   else
#       define TK_arraylength(...) (sizeof(__VA_ARGS__)/sizeof(TK_cliteral_zero(__VA_ARGS__)[0]))
#   endif
#endif



#ifndef TK_stringify
#   define TK_xstringify(...) #__VA_ARGS__
#   define TK_stringify(...) TK_xstringify(__VA_ARGS__)
#endif



#ifndef TK_file
#   ifdef __FILE_NAME__
#       define TK_file() __FILE_NAME__
#   else
#       define TK_file() __FILE__
#   endif
#endif



#ifndef TK_function
#   if TK_has_standard_c99()
#       define TK_function() __func__
#   elif (defined(__GNUC__) && (__GNUC__ >= 2)) || defined(_MSC_VER)
#       define TK_function() __FUNCTION__
#   else
#       define TK_function() 
#   endif
#endif



#ifndef TK_line
#   define TK_line() __LINE__
#endif



#ifndef TK_macro_statement
#   if defined(_MSC_VER) && !defined(__clang__) // Clang likes to pretend it's other compilers...
#       define TK_macro_statement(...) do { __VA_ARGS__; } while (0,0)
#   else
#       define TK_macro_statement(...) do { __VA_ARGS__; } while (0)
#   endif
#endif



#ifndef TK_assert
#   define TK_assert assert
#endif



#ifndef TK_static_assert
#   ifdef __cplusplus
#       define TK_static_assert(...) static_assert(__VA_ARGS__)
#   elif TK_has_standard_c23()
#       define TK_static_assert(...) static_assert(__VA_ARGS__)
#   elif TK_has_standard_c11()
#       define TK_static_assert(...) _Static_assert(__VA_ARGS__, TK_stringify(__VA_ARGS__))
#   else
#       define TK_static_assert(...) 
#   endif
#endif



typedef
    struct TK_LocationMacroBundle TK_LocationMacroBundle;
    struct TK_LocationMacroBundle
{
    const char *function;
    const char *file;
    int         line;
};



#define TK_LocationMacroBundle() (TK_cliteral(TK_LocationMacroBundle){ .function = TK_function(), .file = TK_file(), .line = TK_line() })



#define TK_clamp(n, min, max)                       \
    ({                                              \
        __typeof__(n)  n_ =   (n);                  \
        __typeof__(n_) min_ = (min);                \
        __typeof__(n_) max_ = (max);                \
                                                    \
        n_ < min_ ? min_ : (n_ > max_ ? max_ : n_); \
    })



#ifndef TKM_PI
#   define TKM_PI() 3.14159265358979323846f
#endif



#define TKM_TAU() (TKM_PI() * 2)



#define TK_radians(deg) ((TKM_PI() / 180.f) * (deg))



#define TK_degrees(rad) ((180.f / TKM_PI()) * (rad))



#define TK_forcetype(T, ...) _Generic((__VA_ARGS__), T: (__VA_ARGS__), default: TK_cliteral_zero(T))



#define TK_gluex(x, ...) x##__VA_ARGS__
#define TK_glue(x, ...)  TK_gluex(x, __VA_ARGS__)



void TK_Update();
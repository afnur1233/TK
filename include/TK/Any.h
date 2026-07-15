#pragma once



#include <TK/Math/Vector.h>
#include <TK/String.h>
#include <TK/Basic.h>





#ifndef TK__ENTITY_TYPEDEF
#   define TK__ENTITY_TYPEDEF
    typedef struct TK_Entity TK_Entity;
#endif




typedef enum
{
    TK_NONE,
    TK_INT,
    TK_UINT,
    TK_BOOL,
    TK_FLOAT,
    TK_DOUBLE,
    TK_FACTOR,
    TK_POINTER,
    TK_FUNCTION,
    TK_STRING_SLICE,
    TK_CSTRING,
    TK_STRING,
    TK_VEC2I,
    TK_VEC3I,
    TK_VEC4I,
    TK_VEC2U,
    TK_VEC3U,
    TK_VEC4U,
    TK_VEC2F,
    TK_VEC3F,
    TK_VEC4F,
    TK_VEC2D,
    TK_VEC3D,
    TK_VEC4D,
    TK_ENTITY,
} TK_Type;




typedef
    struct TK_Any TK_Any;
    struct TK_Any
{
    TK_Type type;
    
    union
    {
        int             int_;
        uint            uint_;
        bool            bool_;
        float           float_;
        double          double_;
        float           factor;
        void           *pointer;
        void           *function;
        TK_StringSlice  string_slice;
        char           *cstring;
        TK_String       string;
        TK_vec2i        vec2i;
        TK_vec3i        vec3i;
        TK_vec4i        vec4i;
        TK_vec2u        vec2u;
        TK_vec3u        vec3u;
        TK_vec4u        vec4u;
        TK_vec2f        vec2f;
        TK_vec3f        vec3f;
        TK_vec4f        vec4f;
        TK_vec2d        vec2d;
        TK_vec3d        vec3d;
        TK_vec4d        vec4d;
        TK_Entity      *entity;
    } data;
};



#define TK_NONE()            (TK_cliteral(TK_Any){ TK_NONE })
#define TK_INT(...)          (TK_cliteral(TK_Any){ TK_INT          __VA_OPT__(, .data.int_         = __VA_ARGS__) })
#define TK_UINT(...)         (TK_cliteral(TK_Any){ TK_UINT         __VA_OPT__(, .data.uint_        = __VA_ARGS__) })
#define TK_BOOL(...)         (TK_cliteral(TK_Any){ TK_BOOL         __VA_OPT__(, .data.bool_        = __VA_ARGS__) })
#define TK_FLOAT(...)        (TK_cliteral(TK_Any){ TK_FLOAT        __VA_OPT__(, .data.float_       = __VA_ARGS__) })
#define TK_DOUBLE(...)       (TK_cliteral(TK_Any){ TK_DOUBLE       __VA_OPT__(, .data.double_      = __VA_ARGS__) })
#define TK_FACTOR(...)       (TK_cliteral(TK_Any){ TK_FACTOR       __VA_OPT__(, .data.factor       = __VA_ARGS__) })
#define TK_POINTER(...)      (TK_cliteral(TK_Any){ TK_POINTER      __VA_OPT__(, .data.pointer      = __VA_ARGS__) })
#define TK_FUNCTION(...)     (TK_cliteral(TK_Any){ TK_FUNCTION     __VA_OPT__(, .data.function     = __VA_ARGS__) })
#define TK_STRING_SLICE(...) (TK_cliteral(TK_Any){ TK_STRING_SLICE __VA_OPT__(, .data.string_slice = __VA_ARGS__) })
#define TK_CSTRING(...)      (TK_cliteral(TK_Any){ TK_CSTRING      __VA_OPT__(, .data.cstring      = __VA_ARGS__) })
#define TK_STRING(...)       (TK_cliteral(TK_Any){ TK_STRING       __VA_OPT__(, .data.string       = __VA_ARGS__) })
#define TK_VEC2I(...)        (TK_cliteral(TK_Any){ TK_VEC2I        __VA_OPT__(, .data.vec2i        = __VA_ARGS__) })
#define TK_VEC3I(...)        (TK_cliteral(TK_Any){ TK_VEC3I        __VA_OPT__(, .data.vec3i        = __VA_ARGS__) })
#define TK_VEC4I(...)        (TK_cliteral(TK_Any){ TK_VEC4I        __VA_OPT__(, .data.vec4i        = __VA_ARGS__) })
#define TK_VEC2U(...)        (TK_cliteral(TK_Any){ TK_VEC2U        __VA_OPT__(, .data.vec2u        = __VA_ARGS__) })
#define TK_VEC3U(...)        (TK_cliteral(TK_Any){ TK_VEC3U        __VA_OPT__(, .data.vec3u        = __VA_ARGS__) })
#define TK_VEC4U(...)        (TK_cliteral(TK_Any){ TK_VEC4U        __VA_OPT__(, .data.vec4u        = __VA_ARGS__) })
#define TK_VEC2F(...)        (TK_cliteral(TK_Any){ TK_VEC2F        __VA_OPT__(, .data.vec2f        = __VA_ARGS__) })
#define TK_VEC3F(...)        (TK_cliteral(TK_Any){ TK_VEC3F        __VA_OPT__(, .data.vec3f        = __VA_ARGS__) })
#define TK_VEC4F(...)        (TK_cliteral(TK_Any){ TK_VEC4F        __VA_OPT__(, .data.vec4f        = __VA_ARGS__) })
#define TK_VEC2D(...)        (TK_cliteral(TK_Any){ TK_VEC2D        __VA_OPT__(, .data.vec2d        = __VA_ARGS__) })
#define TK_VEC3D(...)        (TK_cliteral(TK_Any){ TK_VEC3D        __VA_OPT__(, .data.vec3d        = __VA_ARGS__) })
#define TK_VEC4D(...)        (TK_cliteral(TK_Any){ TK_VEC4D        __VA_OPT__(, .data.vec4d        = __VA_ARGS__) })
#define TK_ENTITY(...)       (TK_cliteral(TK_Any){ TK_ENTITY       __VA_OPT__(, .data.entity       = __VA_ARGS__) })

#define TK_SOME(x, ...) TK_SOME_(x __VA_OPT__(,__VA_ARGS__))
#define TK_SOME_(...)                                                                \
    _Generic((__VA_ARGS__),                                                          \
        int:            TK_INT          (TK_forcetype(int,            __VA_ARGS__)), \
        uint:           TK_UINT         (TK_forcetype(uint,           __VA_ARGS__)), \
        bool:           TK_BOOL         (TK_forcetype(bool,           __VA_ARGS__)), \
        float:          TK_FLOAT        (TK_forcetype(float,          __VA_ARGS__)), \
        double:         TK_DOUBLE       (TK_forcetype(double,         __VA_ARGS__)), \
        void *:         TK_POINTER      (TK_forcetype(void *,         __VA_ARGS__)), \
        TK_StringSlice: TK_STRING_SLICE (TK_forcetype(TK_StringSlice, __VA_ARGS__)), \
        char *:         TK_CSTRING      (TK_forcetype(char *,         __VA_ARGS__)), \
        TK_String:      TK_STRING       (TK_forcetype(TK_String,      __VA_ARGS__)), \
        TK_vec2i:       TK_VEC2I        (TK_forcetype(TK_vec2i,       __VA_ARGS__)), \
        TK_vec3i:       TK_VEC3I        (TK_forcetype(TK_vec3i,       __VA_ARGS__)), \
        TK_vec4i:       TK_VEC4I        (TK_forcetype(TK_vec4i,       __VA_ARGS__)), \
        TK_vec2u:       TK_VEC2U        (TK_forcetype(TK_vec2u,       __VA_ARGS__)), \
        TK_vec3u:       TK_VEC3U        (TK_forcetype(TK_vec3u,       __VA_ARGS__)), \
        TK_vec4u:       TK_VEC4U        (TK_forcetype(TK_vec4u,       __VA_ARGS__)), \
        TK_vec2f:       TK_VEC2F        (TK_forcetype(TK_vec2f,       __VA_ARGS__)), \
        TK_vec3f:       TK_VEC3F        (TK_forcetype(TK_vec3f,       __VA_ARGS__)), \
        TK_vec4f:       TK_VEC4F        (TK_forcetype(TK_vec4f,       __VA_ARGS__)), \
        TK_vec2d:       TK_VEC2D        (TK_forcetype(TK_vec2d,       __VA_ARGS__)), \
        TK_vec3d:       TK_VEC3D        (TK_forcetype(TK_vec3d,       __VA_ARGS__)), \
        TK_vec4d:       TK_VEC4D        (TK_forcetype(TK_vec4d,       __VA_ARGS__)), \
        TK_Entity *:    TK_ENTITY       (TK_forcetype(TK_Entity *,    __VA_ARGS__))  \
    )




#define __TK_ANY_01()
#define __TK_ANY_0() TK_NONE()
#define __TK_ANY(...) TK_glue(__TK_ANY_0, __VA_OPT__(1))()
#define TK_ANY(...)__VA_OPT__(TK_SOME(__VA_ARGS__))__TK_ANY(__VA_ARGS__)
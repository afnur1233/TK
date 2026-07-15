#pragma once



#include <TK/Basic.h>





#define TK_vec2_template(T)              \
    {                                    \
        union                            \
        {                                \
            struct { T x, y; };          \
            struct { T u, v; };          \
            struct { T width, height; }; \
                                         \
            T components[2];             \
        };                               \
    }



#define TK_vec3_template(T, vec2)      \
    {                                  \
        union                          \
        {                              \
            struct { T x, y, z;     }; \
            struct { vec2 xy;       }; \
            struct { T _; vec2 yz;  }; \
                                       \
            struct { T r, g, b;     }; \
            struct { vec2 rg;       }; \
            struct { T __; vec2 gb; }; \
                                       \
            T components[3];           \
        };                             \
    }



#define TK_vec4_template(T, vec2, vec3)   \
    {                                     \
        union                             \
        {                                 \
            struct { T x, y, z, w;     }; \
            struct { vec2 xy; vec2 zw; }; \
            struct { T _; vec2 yz;     }; \
            struct { vec3 xyz;         }; \
            struct { T __; vec3 yzw;   }; \
                                          \
            struct { T r, g, b, a;     }; \
            struct { vec2 rg; vec2 ba; }; \
            struct { T ___; vec2 gb;   }; \
            struct { vec3 rgb;         }; \
            struct { T ____; vec2 gba; }; \
                                          \
            T components[4];              \
        };                                \
    }




typedef struct TK_vec2_template(int                        ) TK_vec2i;
typedef struct TK_vec3_template(int,     TK_vec2i          ) TK_vec3i;
typedef struct TK_vec4_template(int,     TK_vec2i, TK_vec3i) TK_vec4i;
typedef struct TK_vec2_template(int8_t                     ) TK_vec2i8;
typedef struct TK_vec3_template(int8_t,  TK_vec2i          ) TK_vec3i8;
typedef struct TK_vec4_template(int8_t,  TK_vec2i, TK_vec3i) TK_vec4i8;
typedef struct TK_vec2_template(uint                       ) TK_vec2u;
typedef struct TK_vec3_template(uint,    TK_vec2u          ) TK_vec3u;
typedef struct TK_vec4_template(uint,    TK_vec2u, TK_vec3u) TK_vec4u;
typedef struct TK_vec2_template(uint8_t                    ) TK_vec2u8;
typedef struct TK_vec3_template(uint8_t, TK_vec2i          ) TK_vec3u8;
typedef struct TK_vec4_template(uint8_t, TK_vec2i, TK_vec3i) TK_vec4u8;
typedef struct TK_vec2_template(float                      ) TK_vec2f;
typedef struct TK_vec3_template(float,   TK_vec2f          ) TK_vec3f;
typedef struct TK_vec4_template(float,   TK_vec2f, TK_vec3f) TK_vec4f;
typedef struct TK_vec2_template(double                     ) TK_vec2d;
typedef struct TK_vec3_template(double,  TK_vec2d          ) TK_vec3d;
typedef struct TK_vec4_template(double,  TK_vec2d, TK_vec3d) TK_vec4d;




#define TKM_vector_add(...)                                                                     \
    ({                                                                                          \
        __typeof__(__VA_ARGS__) args[] = {__VA_ARGS__};                                         \
                                                                                                \
        TK_static_assert(TK_arraylength(args) > 0);                                             \
                                                                                                \
                                                                                                \
        __typeof__(args[0]) out = args[0];                                                      \
                                                                                                \
                                                                                                \
        for (size_t component = 0; component < TK_arraylength(args[0].components); ++component) \
            for (size_t i = 1; i < TK_arraylength(args); ++i)                                   \
                out.components[component] += args[i].components[component];                     \
                                                                                                \
        out;                                                                                    \
    })




#define TKM_vector_sub(...)                                                                     \
    ({                                                                                          \
        __typeof__(__VA_ARGS__) args[] = {__VA_ARGS__};                                         \
                                                                                                \
        TK_static_assert(TK_arraylength(args) > 0);                                             \
                                                                                                \
                                                                                                \
        __typeof__(args[0]) out = args[0];                                                      \
                                                                                                \
                                                                                                \
        for (size_t component = 0; component < TK_arraylength(args[0].components); ++component) \
            for (size_t i = 1; i < TK_arraylength(args); ++i)                                   \
                out.components[component] -= args[i].components[component];                     \
                                                                                                \
        out;                                                                                    \
    })




#define TKM_vector_mul(...)                                                                     \
    ({                                                                                          \
        __typeof__(__VA_ARGS__) args[] = {__VA_ARGS__};                                         \
                                                                                                \
        TK_static_assert(TK_arraylength(args) > 0);                                             \
                                                                                                \
                                                                                                \
        __typeof__(args[0]) out = args[0];                                                      \
                                                                                                \
                                                                                                \
        for (size_t component = 0; component < TK_arraylength(args[0].components); ++component) \
            for (size_t i = 1; i < TK_arraylength(args); ++i)                                   \
                out.components[component] *= args[i].components[component];                     \
                                                                                                \
        out;                                                                                    \
    })




#define TKM_vector_mul_num(vector, num)                                                         \
    ({                                                                                          \
        __typeof__(vector)                vector_ = (vector);                                   \
        __typeof__(vector_.components[0]) num_   = (num);                                       \
                                                                                                \
                                                                                                \
        for (size_t component = 0; component < TK_arraylength(vector_.components); ++component) \
            vector_.components[component] *= num_;                                              \
                                                                                                \
        vector_;                                                                                \
    })




#define TKM_vector_div(...)                                                                     \
    ({                                                                                          \
        __typeof__(__VA_ARGS__) args[] = {__VA_ARGS__};                                         \
                                                                                                \
        TK_static_assert(TK_arraylength(args) > 0);                                             \
                                                                                                \
                                                                                                \
        __typeof__(args[0]) out = args[0];                                                      \
                                                                                                \
                                                                                                \
        for (size_t component = 0; component < TK_arraylength(args[0].components); ++component) \
            for (size_t i = 1; i < TK_arraylength(args); ++i)                                   \
                out.components[component] /= args[i].components[component];                     \
                                                                                                \
        out;                                                                                    \
    })




#define TKM_vector_div_num(vector, num)                                                         \
    ({                                                                                          \
        __typeof__(vector)                vector_ = (vector);                                   \
        __typeof__(vector_.components[0]) num_   = (num);                                       \
                                                                                                \
                                                                                                \
        for (size_t component = 0; component < TK_arraylength(vector_.components); ++component) \
            vector_.components[component] /= num_;                                              \
                                                                                                \
        vector_;                                                                                \
    })




#define TKM_vector_dot(...)                                                                     \
    ({                                                                                          \
        __typeof__(__VA_ARGS__) args[2] = {__VA_ARGS__};                                        \
                                                                                                \
                                                                                                \
        __typeof__(args[0].components[0]) out = 0;                                              \
                                                                                                \
                                                                                                \
        for (size_t component = 0; component < TK_arraylength(args[0].components); ++component) \
            out += args[0].components[component] * args[1].components[component];               \
                                                                                                \
                                                                                                \
        out;                                                                                    \
    })




#define TKM_vector_dot2(...) TKM_vector_dot(__VA_ARGS__, __VA_ARGS__)




#define TKM_vector_length(...) sqrt(TKM_vector_dot(__VA_ARGS__, __VA_ARGS__))




#define TKM_vector_normalize(...)                                    \
    ({                                                               \
        __typeof__(__VA_ARGS__) vector__ __VA_OPT__( = __VA_ARGS__); \
                                                                     \
        TKM_vector_div_num(vector__, TKM_vector_length(vector__));   \
    })


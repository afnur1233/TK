#pragma once



#include <TK/Basic.h>




#define TK_range_template(...)                 \
    {                                          \
        union                                  \
        {                                      \
            struct{ __VA_ARGS__ begin, end; }; \
            struct{ __VA_ARGS__ near, far; };  \
        };                                     \
    }




typedef struct TK_range_template(float) TK_rangef;
typedef struct TK_range_template(int)   TK_rangei;
typedef struct TK_range_template(uint)  TK_rangeu;



#define TK_fixrange(...)                                                                    \
    ({                                                                                      \
        const __typeof__(__VA_ARGS__) range_ = (__VA_ARGS__);                               \
                                                                                            \
        (range_.begin > range_.end)                                                         \
            ? TK_cliteral(__typeof__(range_)){ .begin = range_.end,   .end = range_.begin } \
            : TK_cliteral(__typeof__(range_)){ .begin = range_.begin, .end = range_.end   } \
            ;                                                                               \
    })




#define TK_rangelength(...)                                   \
    ({                                                        \
        const __typeof__(__VA_ARGS__) range_ = (__VA_ARGS__); \
                                                              \
        (range_.begin > range_.end)                           \
            ? (range_.begin - range_.end)                     \
            : (range_.end - range_.begin)                     \
            ;                                                 \
    })
#pragma once



#include <TK/Basic.h>





typedef enum {
    TK_ALLOCATOR_MODE_ALLOCATE,
    TK_ALLOCATOR_MODE_REALLOCATE,
    /* DO NOT USE UNTIL THE LIBC ALLOCATOR IS REMOVED! */
    TK_ALLOCATOR_MODE_REALLOCATE_BACKWARD,
    TK_ALLOCATOR_MODE_FREE,
} TK_AllocatorMode;



typedef struct TK_Allocator TK_Allocator;



typedef void *TK_AllocatorFunction(
    TK_LocationMacroBundle  caller,
    TK_Allocator           *allocator,
    TK_AllocatorMode        mode,
    size_t                  size,
    void                   *block
);



struct TK_Allocator { TK_AllocatorFunction *function; };




#define TK_allocate(allocator, ...)            (allocator)->function(TK_LocationMacroBundle(), (allocator), TK_ALLOCATOR_MODE_ALLOCATE, (__VA_ARGS__), NULL)
#define TK_reallocate(allocator, ...)          (allocator)->function(TK_LocationMacroBundle(), (allocator), TK_ALLOCATOR_MODE_REALLOCATE, __VA_ARGS__)
#define TK_reallocate_backward(allocator, ...) (allocator)->function(TK_LocationMacroBundle(), (allocator), TK_ALLOCATOR_MODE_REALLOCATE_BACKWARD, __VA_ARGS__)
#define TK_free(allocator, ...)                (allocator)->function(TK_LocationMacroBundle(), (allocator), TK_ALLOCATOR_MODE_FREE, 0, (__VA_ARGS__))




void *TK_LibCAllocatorFunction(
    TK_LocationMacroBundle  caller,
    TK_Allocator           *allocator,
    TK_AllocatorMode        mode,
    size_t                  size,
    void                   *block
);
#pragma once



#include <TK/Allocator.h>
#include <TK/Basic.h>






// A view into a null terminated UTF-8 string. The view might not include the
// null terminator but the string must have it. The string is not guarenteed to
// be on heap as it may be a literal. The slice must be inside the string.
// 
// 
// pneumonoultramicroscopicsilicovolcanoconiosis
// --------------------------------------------- string
//         ----- slice
// 
// 
// TK_StringSlice guarantees the following:
// 
//    items != NULL where length > 0
//    items is null terminated
// 
typedef
    struct TK_StringSlice TK_StringSlice;
    struct TK_StringSlice
{
    char *items;
    size_t length;
};





// A heap allocated null terminated UTF-8 string. The buffer is guarenteed to be in the
// heap. The buffer has an extra byte for the null terminator, so it's (capacity + 1) long.
// 
// 
// TK_String guarantees the following:
// 
//    allocator       != NULL
//    items           != NULL where capacity > 0
//    items           == NULL where capacity == 0
//    capacity + 1    == allocation_sizeof(buffer)
//    length          <= capacity
//    items[length]   == '\0'
//    items[capacity] == '\0'
// 
typedef
    struct TK_String TK_String;
    struct TK_String
{
    union
    {
        struct
        {
            char *items;
            size_t length;
        };
        
        TK_StringSlice slice;
    };
    
     size_t capacity;
     TK_Allocator *allocator;
};




#define TK_assert_string_invariants(...)             \
    TK_macro_statement(                              \
        TK_String *str__ = (__VA_ARGS__);            \
                                                     \
        TK_assert(str__);                            \
        TK_assert(str__->allocator);                 \
        TK_assert(str__->capacity >= str__->length); \
                                                     \
        if (str__->capacity > 0)                     \
            TK_assert(str__->items);                 \
        else                                         \
            TK_assert(!str__->items);                \
    )




#define TK_StringSliceLiteral(...) (TK_cliteral(TK_StringSlice){ .items = (__VA_ARGS__), .length = sizeof(__VA_ARGS__) - 1 })



bool TK_AreStringSlicesEq(TK_StringSlice a, TK_StringSlice b);
TK_StringSlice TK_StringSliceFromCstr(char *s);



bool TK_ResizeString(TK_String *str, size_t capacity);
bool TK_AppendSliceToString(TK_String *str, TK_StringSlice slice);
void TK_DestroyString(TK_String *str);
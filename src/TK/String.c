#include <TK/String.h>







TK_StringSlice TK_StringSliceFromCstr(char *s)
{
    TK_assert(s);
    
    return TK_cliteral(TK_StringSlice){
        .items = s,
        .length = strlen(s),
    };
}




bool TK_ResizeString(TK_String *str, size_t capacity)
{
    TK_assert_string_invariants(str);
    
    
    if (str->items)
        str->items = TK_reallocate(str->allocator, (capacity + 1) * sizeof(*str->items), str->items);
    else
    {
        str->items = TK_allocate(str->allocator, (capacity + 1) * sizeof(*str->items));
        str->length = 0;
    }
    
    
    if (!str->items)
    {
        str->capacity = 0;
        str->length = 0;
        
        return false;
    }
    
    
    if (str->length > capacity)
        str->length = capacity;
    
    str->capacity = capacity;
    str->items[capacity] = '\0';
    str->items[str->length] = '\0';
    
    
    return true;
}



bool TK_AppendSliceToString(TK_String *str, TK_StringSlice slice)
{
    TK_assert_string_invariants(str);
    
    
    size_t new_capacity = str->length + slice.length;
    if (new_capacity > str->length)
        if (!TK_ResizeString(str, new_capacity))
            return false;
    
    
    for (size_t i = 0; i < slice.length; ++i)
    {
        char byte = slice.items[i];
        
        if (byte == '\0')
            break;
        
        str->items[str->length++] = byte;
    }
    
    str->items[str->length] = '\0';
    
    
    return true;
}



void TK_DestroyString(TK_String *str)
{
    TK_assert_string_invariants(str);
    
    
    if (str->items)
        TK_free(str->allocator, str->items);
    
    
    str->items    = NULL;
    str->length   = 0;
    str->capacity = 0;
}



bool TK_AreStringSlicesEq(TK_StringSlice a, TK_StringSlice b)
{
    if (a.length != b.length)
        return false;
    
    if (a.length > 0 && !a.items)
        return false;
    
    if (b.length > 0 && !b.items)
        return false;
    
    
    for (size_t i = 0; i < a.length; ++i)
        if (a.items[i] != b.items[i])
            return false;
    
    
    return true;
}
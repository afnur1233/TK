#include <TK/Allocator.h>





void *TK_LibCAllocatorFunction(
    TK_LocationMacroBundle  caller,
    TK_Allocator           *allocator,
    TK_AllocatorMode        mode,
    size_t                  size,
    void                   *block
)
{
    TK_assert(allocator);
    TK_discard(caller);
    
    
    switch (mode)
    {
        case TK_ALLOCATOR_MODE_ALLOCATE:
            TK_assert(size > 0);
            TK_assert(!block);
            
            return malloc(size);
        
        case TK_ALLOCATOR_MODE_REALLOCATE:
            TK_assert(size > 0);
            TK_assert(block);
            
            return realloc(block, size);
        
        case TK_ALLOCATOR_MODE_REALLOCATE_BACKWARD:
            TK_assert(false && "Can not reallocate backwards with libc");
            TK_unreachable();
            return NULL;
        
        case TK_ALLOCATOR_MODE_FREE:
            TK_assert(size == 0);
            TK_assert(block);
            
            free(block);
            
            return NULL;
    }
}
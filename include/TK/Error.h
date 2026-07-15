#pragma once



#include <TK/Containers/Queue.h>
#include <TK/String.h>





typedef TK_StringSlice TK_Error;



typedef struct TK_Queue(TK_Error) TK_ErrorQueue;



#define TK_ERROR_ALLOCATION TK_StringSliceLiteral("Allocation failed.")



#define TK_ERROR_NO_ALLOCATOR TK_StringSliceLiteral("No allocator was provided.")



#define TK_ERROR_NO_GRAPHICS_CONTEXT TK_StringSliceLiteral("No graphics context was provided.")



#define TK_ERROR_NO_WINDOW TK_StringSliceLiteral("No window was provided.")



#define TK_ERROR_BAD_STR_SLICE TK_StringSliceLiteral("String slice of length > 0 has no pointer.")



#define TK_ERROR_OPEN_FOR_READ_NOR_WRITE TK_StringSliceLiteral("Attempt to open for read nor write.")
#pragma once



#include <TK/Containers/Queue.h>
#include <TK/Math/Vector.h>
#include <TK/Allocator.h>
#include <TK/Error.h>
#include <TK/Basic.h>
#include <TK/Any.h>





typedef struct TK_WindowEvent                               TK_WindowEvent;
typedef struct TK_WindowFlags                               TK_WindowFlags;
typedef struct TK_WindowEventQueue TK_Queue(TK_WindowEvent) TK_WindowEventQueue;
typedef struct TK_Window                                    TK_Window;
typedef struct TK_WindowCreateInfo                          TK_WindowCreateInfo;




struct __attribute__((__packed__)) TK_WindowFlags
{
    bool resizable     : 1;
    bool fullscreen    : 1;
    bool not_focusable : 1;
    bool vsync         : 1;
};




typedef enum
{
    TK_EVENT_WINDOW_QUIT,
    TK_EVENT_WINDOW_FOCUS_GAINED,
    TK_EVENT_WINDOW_FOCUS_LOST,
    TK_EVENT_WINDOW_MOUSE_ENTER,
    TK_EVENT_WINDOW_MOUSE_LEAVE,
    TK_EVENT_WINDOW_MAXIMIZED,
    TK_EVENT_WINDOW_MINIMIZED,
    TK_EVENT_WINDOW_RESIZED,
    TK_EVENT_WINDOW_MOVED,
} TK_WindowEventType;




typedef enum
{
    TK_WINDOW_TYPE_DEFAULT,
    TK_WINDOW_TYPE_POPUP_MENU,
    TK_WINDOW_TYPE_TOOLTIP,
} TK_WindowType;




struct TK_WindowEvent
{
    TK_WindowEventType type;
    TK_Window *window;
    TK_Any data1;
};




struct TK_Window
{
    TK_Allocator        *allocator;
    TK_ErrorQueue       *errorQueue;
    TK_WindowEventQueue *eventQueue;
    TK_WindowFlags       flags;
    char                 internal[0];
};




struct TK_WindowCreateInfo
{
    TK_Allocator        *allocator;
    TK_ErrorQueue       *errorQueue;
    TK_WindowEventQueue *eventQueue;
    TK_Window           *parent;
    TK_WindowType        type;
    TK_vec2u             size;
    TK_StringSlice       title;
    TK_WindowFlags       flags;
};




#define TK_CreateWindow(...) TK_CreateWindowOPT((TK_WindowCreateInfo){__VA_ARGS__})




TK_Window *TK_CreateWindowOPT   (TK_WindowCreateInfo createInfo);
void       TK_DestroyWindow     (TK_Window *window);
TK_vec2u   TK_GetWindowSize     (TK_Window *window);
void       TK_SwapBuffers       (TK_Window *window);
void      *TK_GL_GetProcAddress (const char *proc);
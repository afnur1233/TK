#include <TK/Platform/SDL/Common.h>
#include <TK/Window.h>
#include <SDL3/SDL.h>




typedef
    struct TK_WindowSDLInternal TK_WindowSDLInternal;
    struct TK_WindowSDLInternal
{
    SDL_Window *sdlWindow;
    SDL_GLContext glContext;
};




TK_Window *TK_CreateWindowOPT(TK_WindowCreateInfo createInfo)
{
    {
        if (createInfo.allocator == NULL)
        {
            if (createInfo.errorQueue)
                TK_enqueue(createInfo.errorQueue, TK_ERROR_NO_ALLOCATOR);
            
            return NULL;
        }
        
        
        if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
        {
            if (createInfo.errorQueue)
                TK_enqueue(createInfo.errorQueue, TK_StringSliceFromCstr(cast(char *)SDL_GetError()));
            
            return NULL;
        }
    }
    
    
    SDL_Window *sdlWindow = NULL;
    SDL_GLContext sdlGLContext = NULL;
    TK_Window *window = NULL;
    {
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        
        char *cstrTitle = alloca(sizeof(*cstrTitle) * (createInfo.title.length + 1));
        {
            memcpy(cstrTitle, createInfo.title.items, createInfo.title.length);
            cstrTitle[createInfo.title.length] = '\0';
        }
        
        
        SDL_WindowFlags sdlFlags = SDL_WINDOW_OPENGL;
        {
            if (createInfo.type == TK_WINDOW_TYPE_POPUP_MENU) sdlFlags |= SDL_WINDOW_POPUP_MENU;
            if (createInfo.type == TK_WINDOW_TYPE_TOOLTIP)    sdlFlags |= SDL_WINDOW_TOOLTIP;
            if (createInfo.flags.resizable)                   sdlFlags |= SDL_WINDOW_RESIZABLE;
            if (createInfo.flags.fullscreen)                  sdlFlags |= SDL_WINDOW_FULLSCREEN;
            if (createInfo.flags.not_focusable)               sdlFlags |= SDL_WINDOW_NOT_FOCUSABLE;
        }
        
        
        sdlWindow = SDL_CreateWindow(cstrTitle, createInfo.size.width, createInfo.size.height, sdlFlags);
        SDL_GL_SetSwapInterval(createInfo.flags.vsync);
        
        if (!sdlWindow)
        {
            if (createInfo.errorQueue)
                TK_enqueue(createInfo.errorQueue, TK_StringSliceFromCstr(cast(char *)SDL_GetError()));
            
            goto fail;
        }
        
        sdlGLContext = SDL_GL_CreateContext(sdlWindow);
        
        if (createInfo.parent)
        {
            TK_WindowSDLInternal *parentInternal = cast(void *)&createInfo.parent->internal;
            
            
            if (!SDL_SetWindowParent(sdlWindow, parentInternal->sdlWindow))
            {
                if (createInfo.errorQueue)
                    TK_enqueue(createInfo.errorQueue, TK_StringSliceFromCstr(cast(char *)SDL_GetError()));
                
                goto fail;
            }
        }
    }
    
    
    {
        window = TK_allocate(createInfo.allocator, sizeof(*window) + sizeof(TK_WindowSDLInternal));
        if (!window)
        {
            if (createInfo.errorQueue)
                TK_enqueue(createInfo.errorQueue, TK_ERROR_ALLOCATION);
            
            goto fail;
        }
        
        
        SDL_PropertiesID sdlWindowProperties = SDL_GetWindowProperties(sdlWindow);
        SDL_SetPointerProperty(sdlWindowProperties, TK__SDL_PROP_TK_WINDOW, window);
        
        
        TK_WindowSDLInternal *internal = cast(void *)&window->internal;
        
        internal->sdlWindow = sdlWindow;
        internal->glContext = sdlGLContext;
        window->allocator   = createInfo.allocator;
        window->errorQueue  = createInfo.errorQueue;
        window->eventQueue  = createInfo.eventQueue;
        window->flags       = createInfo.flags;
        
        
        TK__SDLState.videoSubsystemUsers++;
    }
    
    
    return window;
    
fail:
    if (sdlWindow)
        SDL_DestroyWindow(sdlWindow);
    
    if (window)
        TK_DestroyWindow(window);
    
    if (TK__SDLState.videoSubsystemUsers == 0)
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    
    return NULL;
}



void TK_DestroyWindow(TK_Window *window)
{
    if (!window)
        return;
    
    if (!window)
    {
        if (window->errorQueue)
            TK_enqueue(window->errorQueue, TK_ERROR_NO_ALLOCATOR);
        
        return;
    }
    
    
    TK_WindowSDLInternal *internal = cast(void *)&window->internal;
    
    SDL_DestroyWindow(internal->sdlWindow);
    SDL_GL_DestroyContext(internal->glContext);
    
    
    TK_free(window->allocator, window);
    
    
    TK__SDLState.videoSubsystemUsers--;
    
    if (TK__SDLState.videoSubsystemUsers == 0)
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
}



TK_vec2u TK_GetWindowSize(TK_Window *window)
{
    if (!window)
        return TK_cliteral(TK_vec2u){ .width = 0, .height = 0 };
    
    TK_WindowSDLInternal *internal = cast(void *)&window->internal;
    
    
    int x, y;
    SDL_GetWindowSize(internal->sdlWindow, &x, &y);
    
    
    return TK_cliteral(TK_vec2u){ .width = x, .height = y };
}



void TK_SwapBuffers(TK_Window *window)
{
    TK_WindowSDLInternal *internal = cast(void *)&window->internal;
    
    SDL_GL_SwapWindow(internal->sdlWindow);
}



void *TK_GL_GetProcAddress(const char *proc)
{
    return SDL_GL_GetProcAddress(proc);
}
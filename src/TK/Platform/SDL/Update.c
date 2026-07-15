#include <TK/Platform/SDL/Common.h>
#include <TK/Window.h>
#include <TK/Any.h>
#include <SDL3/SDL.h>




void TK_Update()
{
    SDL_Event sdlEvent = TK_cliteral_zero();
    
    while (SDL_PollEvent(&sdlEvent))
    {
        SDL_Window *sdlWindow = SDL_GetWindowFromEvent(&sdlEvent);
        SDL_PropertiesID sdlWindowProperties = 0;
        TK_Window *window = NULL;
        
        if (sdlWindow)
        {
            sdlWindowProperties = SDL_GetWindowProperties(sdlWindow);
            window = SDL_GetPointerProperty(sdlWindowProperties, TK__SDL_PROP_TK_WINDOW, NULL);
        }
        
        
        
        switch (sdlEvent.type)
        {
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                if (window->eventQueue)
                    TK_enqueue(window->eventQueue, { .type = TK_EVENT_WINDOW_QUIT });
                break;
            
            case SDL_EVENT_WINDOW_FOCUS_GAINED:
                if (window->eventQueue)
                    TK_enqueue(window->eventQueue, { .type = TK_EVENT_WINDOW_FOCUS_GAINED });
                break;
            
            case SDL_EVENT_WINDOW_FOCUS_LOST:
                if (window->eventQueue)
                    TK_enqueue(window->eventQueue, { .type = TK_EVENT_WINDOW_FOCUS_LOST });
                break;
                
            case SDL_EVENT_WINDOW_MOUSE_ENTER:
                if (window->eventQueue)
                    TK_enqueue(window->eventQueue, { .type = TK_EVENT_WINDOW_MOUSE_ENTER });
                break;
            
            case SDL_EVENT_WINDOW_MOUSE_LEAVE:
                if (window->eventQueue)
                    TK_enqueue(window->eventQueue, { .type = TK_EVENT_WINDOW_MOUSE_LEAVE });
                break;
            
            case SDL_EVENT_WINDOW_MAXIMIZED:
                if (window->eventQueue)
                    TK_enqueue(window->eventQueue, { .type = TK_EVENT_WINDOW_MAXIMIZED });
                break;
            
            case SDL_EVENT_WINDOW_MINIMIZED:
                if (window->eventQueue)
                    TK_enqueue(window->eventQueue, { .type = TK_EVENT_WINDOW_MINIMIZED });
                break;
            
            case SDL_EVENT_WINDOW_RESIZED:
                if (window->eventQueue)
                {
                    TK_enqueue(window->eventQueue, {
                        .type = TK_EVENT_WINDOW_RESIZED,
                        .data1 = TK_VEC2U({
                            .width  = sdlEvent.window.data1,
                            .height = sdlEvent.window.data2
                        })
                    });
                }
                break;
            
            case SDL_EVENT_WINDOW_MOVED:
                if (window->eventQueue)
                    TK_enqueue(window->eventQueue, { .type = TK_EVENT_WINDOW_MOVED });
                break;
        }
    }
}

#pragma once
#include <TK/Basic.h>



typedef
    struct TK__SDLStateType TK__SDLStateType;
    struct TK__SDLStateType
{
    size_t videoSubsystemUsers;
};



extern TK__SDLStateType TK__SDLState;




#define TK__SDL_PROP_TK_WINDOW "TK.window"
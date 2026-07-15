#pragma once


#include <TK/Allocator.h>
#include <TK/Window.h>
#include <TK/Entity.h>
#include <TK/Error.h>





typedef struct TK_Renderer TK_Renderer;




typedef
    struct TK_RendererCreateInfo TK_RendererCreateInfo;
    struct TK_RendererCreateInfo
{
    TK_Allocator *allocator;
    TK_ErrorQueue *errorQueue;
    TK_Window *window;
};



#define TK_CreateRenderer(...) TK_CreateRendererOPT(TK_cliteral(TK_RendererCreateInfo){__VA_ARGS__})



TK_Renderer *TK_CreateRendererOPT(TK_RendererCreateInfo createInfo);
void TK_DestroyRenderer(TK_Renderer *renderer);
void TK_RenderPresent(TK_Renderer *renderer);
void TK_RenderScene(TK_Renderer *renderer, TK_Entity *sceneRoot);
#include <TK/Renderer.h>
#include <TK/Entity.h>
#include <TK/Window.h>




typedef
    struct MyScene MyScene;
    struct MyScene
{
    TK_Entity *root;
    TK_Entity *floor;
    TK_Entity *box;
    TK_Entity *boxChild1;
    TK_Entity *boxChild2;
    TK_Entity *boxChild3;
};





int main()
{
    TK_Allocator allocator               = { TK_LibCAllocatorFunction };
    TK_ErrorQueue errorQueue             = TK_AllocateQueueOnStack(errorQueue, 10);
    TK_WindowEventQueue windowEventQueue = TK_AllocateQueueOnStack(windowEventQueue, 64);
    TK_EntityAllocator entityAllocator   = { .allocator = &allocator, .errorQueue = &errorQueue };
    
    TK_Renderer *renderer = NULL;
    TK_Window *gameWindow = NULL;
    
    
    
    gameWindow = TK_CreateWindow(
        .allocator       = &allocator,
        .errorQueue      = &errorQueue,
        .eventQueue      = &windowEventQueue,
        .title           = TK_StringSliceLiteral("Le Game"),
        .size.width      = 640,
        .size.height     = 480,
        .flags.resizable = true,
        .flags.vsync     = true,
    );
    if (!gameWindow) goto cleanup;
    
    
    
    renderer = TK_CreateRenderer(
        .allocator  = &allocator,
        .window     = gameWindow,
        .errorQueue = &errorQueue
    );
    if (!renderer) goto cleanup;
    
    
    
    MyScene myScene = {0};
    {
        myScene.root = TK_CreateEntity(&entityAllocator, NULL, .nickname = TK_StringSliceLiteral("root"));
        if (!myScene.root) goto cleanup;
        
        
        myScene.floor = TK_CreateEntity(&entityAllocator, myScene.root,
            .nickname = TK_StringSliceLiteral("Floor"),
            .properties = TK_PropertiesTemplate(
                { TK_PROP_ENTITY_POSITION3, TK_VEC3F({ +0.f,  -5.f,  +0.f })  },
                { TK_PROP_ENTITY_SCALE3,    TK_VEC3F({ +30.f, +0.1f, +30.f }) },
                { TK_PROP_ENTITY_BOX,       TK_BOOL(true) },
            )
        );
        if (!myScene.floor) goto cleanup;
        
        
        myScene.box = TK_CreateEntity(&entityAllocator, myScene.root,
            .nickname = TK_StringSliceLiteral("Box"),
            .properties = TK_PropertiesTemplate(
                { TK_PROP_ENTITY_SCALE3, TK_VEC3F({ +1.5f, +1.5f, +1.5f }) },
                { TK_PROP_ENTITY_BOX,    TK_BOOL(true) },
            )
        );
        if (!myScene.box) goto cleanup;
        
        
        myScene.boxChild1 = TK_CreateEntity(&entityAllocator, myScene.box,
            .nickname = TK_StringSliceLiteral("Box Child 1"),
            .properties = TK_PropertiesTemplate(
                { TK_PROP_ENTITY_SCALE3, TK_VEC3F({0.5f, 0.5f, 0.5f}) },
                { TK_PROP_ENTITY_BOX,    TK_BOOL(true) },
            )
        );
        if (!myScene.boxChild1) goto cleanup;
        
        
        myScene.boxChild2 = TK_CreateEntity(&entityAllocator, myScene.box,
            .nickname = TK_StringSliceLiteral("Box Child 2"),
            .properties = TK_PropertiesTemplate(
                { TK_PROP_ENTITY_SCALE3, TK_VEC3F({0.5f, 0.5f, 0.5f}) },
                { TK_PROP_ENTITY_BOX,    TK_BOOL(true) },
            )
        );
        if (!myScene.boxChild2) goto cleanup;
        
        
        myScene.boxChild3 = TK_CreateEntity(&entityAllocator, myScene.box,
            .nickname = TK_StringSliceLiteral("Box Child 3"),
            .properties = TK_PropertiesTemplate(
                { TK_PROP_ENTITY_SCALE3, TK_VEC3F({0.5f, 0.5f, 0.5f}) },
                { TK_PROP_ENTITY_BOX,    TK_BOOL(true) },
            )
        );
        if (!myScene.boxChild3) goto cleanup;
    }
    
    
    
    int frame = 0;
    for (bool running = true; running;)
    {
        TK_Update();
        
        while (windowEventQueue.length > 0)
        {
            TK_WindowEvent event = TK_dequeue(&windowEventQueue);
            
            if (event.type == TK_EVENT_WINDOW_QUIT)
                running = false;
        }
        
        
        const float sinFrame  = sin(cast(float)frame / 50.f);
        const float cosFrame  = cos(cast(float)frame / 50.f);
        const float cosFrame2 = cos(cast(float)frame / 50.f + 1.f);
        
        TK_SetProperty(&myScene.box->properties,       TK_PROP_ENTITY_POSITION3, TK_VEC3F({ sinFrame,  0.f,      0.f       }));
        TK_SetProperty(&myScene.boxChild1->properties, TK_PROP_ENTITY_POSITION3, TK_VEC3F({ 0.f,       sinFrame, 0.f       }));
        TK_SetProperty(&myScene.boxChild2->properties, TK_PROP_ENTITY_POSITION3, TK_VEC3F({ cosFrame,  0.f,      0.f       }));
        TK_SetProperty(&myScene.boxChild3->properties, TK_PROP_ENTITY_POSITION3, TK_VEC3F({ 0.f,       0.f,      cosFrame2 }));
        
        TK_RenderScene(renderer, myScene.root);
        
        TK_RenderPresent(renderer);
        frame++;
    }
    
    
    
cleanup:
    TK_DestroyEntities(&entityAllocator);
    TK_DestroyRenderer(renderer);
    TK_DestroyWindow(gameWindow);
    
    while (errorQueue.length)
    {
        TK_Error error = TK_dequeue(&errorQueue);
        
        printf("ERROR -- %.*s\n", cast(int)error.length, error.items);
    }
}
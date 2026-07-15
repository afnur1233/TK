#pragma once



#include <TK/Properties.h>
#include <TK/Allocator.h>
#include <TK/Basic.h>



#define TK_PROP_ENTITY_POSITION3         TK_StringSliceLiteral("TK.entity.position3")
#define TK_PROP_ENTITY_SCALE3            TK_StringSliceLiteral("TK.entity.scale3")
#define TK_PROP_ENTITY_BOX               TK_StringSliceLiteral("TK.entity.box")
#define TK_PROP_ENTITY_POINT_LIGHT_POWER TK_StringSliceLiteral("TK.entity.point_light.power")




#ifndef TK__ENTITY_TYPEDEF /* TK_Entity needs to be defined in both TK/Properties.h and TK/Entity.h */
#   define TK__ENTITY_TYPEDEF
    typedef struct TK_Entity TK_Entity;
#endif

typedef struct TK_EntityTemplate  TK_EntityTemplate;
typedef struct TK_EntityAllocator TK_EntityAllocator;



struct TK_Entity
{
    TK_Entity *next;
    TK_Entity *prev;
    TK_Entity *parent;
    TK_Entity *children;
    size_t child_count;
    TK_Properties properties;
    TK_StringSlice nickname;
    bool used;
};



struct TK_EntityTemplate
{
    struct { TK_EntityTemplate *items; size_t length; } children;
    TK_PropertiesTemplate properties;
    TK_StringSlice nickname;
};



struct TK_EntityAllocator
{
    TK_Allocator *allocator;
    TK_ErrorQueue *errorQueue;
    TK_Entity *items;
    size_t capacity;
    size_t load;
};



#define TK_EntityTemplateChildren(...)                                \
    ((__typeof__(TK_cliteral_zero(TK_EntityTemplate).children)){      \
        .items = ((TK_EntityTemplate[]){__VA_ARGS__}),                \
        .length = TK_arraylength((TK_EntityTemplate[]){__VA_ARGS__}), \
    })



#define TK_CreateEntity(allocator, parent, ...) TK_InstanceEntityTemplate((allocator), (parent), (TK_EntityTemplate){__VA_ARGS__})



void TK_ParentEntity(TK_Entity *child, TK_Entity *parent);
TK_Entity *TK_InstanceEntityTemplate(TK_EntityAllocator *allocator, TK_Entity *parent, TK_EntityTemplate template);
void TK_ClearAllEntities(TK_EntityAllocator *allocator);
void TK_DestroyEntities(TK_EntityAllocator *allocator);
void TK_DestroyEntity(TK_Entity *entity);
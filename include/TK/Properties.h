#pragma once



#include <TK/Math/Vector.h>
#include <TK/Allocator.h>
#include <TK/String.h>
#include <TK/Error.h>
#include <TK/Any.h>




typedef
    struct TK_Property TK_Property;
    struct TK_Property
{
    TK_StringSlice name;
    TK_Any         value;
    bool           strict;
};




typedef
    struct TK_PropertiesTemplate TK_PropertiesTemplate;
    struct TK_PropertiesTemplate
{
    TK_Property *properties;
    int propertyCount;
};




typedef
    struct TK_PropertiesCreateInfo TK_PropertiesCreateInfo;
    struct TK_PropertiesCreateInfo
{
    TK_Allocator *allocator;
    TK_ErrorQueue *errorQueue;
    TK_PropertiesTemplate template_;
};





typedef
    struct TK_Properties TK_Properties;
    struct TK_Properties
{
    TK_Allocator *allocator;
    TK_ErrorQueue *errorQueue;
    TK_Property *items;
    int length;
    int capacity;
};




#define TK_PropertiesTemplate(...)                                                \
    (TK_cliteral(TK_PropertiesTemplate){                                          \
        .properties = TK_cliteral(TK_Property[]){__VA_ARGS__},                    \
        .propertyCount = TK_arraylength(TK_cliteral(TK_Property[]){__VA_ARGS__}), \
    })




#define TK_CreateProperties(allocator_, errorQueue_, ...) \
    TK_CreatePropertiesOPT((TK_PropertiesCreateInfo){     \
        .allocator = (allocator_),                        \
        .errorQueue = (errorQueue_),                      \
        .template_ = TK_PropertiesTemplate(__VA_ARGS__),  \
    })




TK_Properties TK_CreatePropertiesOPT(TK_PropertiesCreateInfo info);
void TK_DestroyProperties(TK_Properties *properties);
bool TK_GetProperty(TK_Properties *properties, TK_StringSlice name, TK_Any *value);
bool TK_SetProperty(TK_Properties *properties, TK_StringSlice name, TK_Any value);
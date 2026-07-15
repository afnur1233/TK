#include <TK/Properties.h>





TK_Properties TK_CreatePropertiesOPT(TK_PropertiesCreateInfo info)
{
    if (!info.allocator)
    {
        if (info.errorQueue)
            TK_enqueue(info.errorQueue, TK_ERROR_NO_ALLOCATOR);
        
        return TK_cliteral_zero(TK_Properties);
    }
    
    if (info.template_.propertyCount > 0 && !info.template_.properties)
    {
        if (info.errorQueue)
            TK_enqueue(info.errorQueue, TK_StringSliceLiteral("info.template_.properties was not set."));
        
        return TK_cliteral_zero(TK_Properties);
    }
    
    
    TK_Properties properties = { .allocator = info.allocator, .errorQueue = info.errorQueue };
    
    
    for (int i = 0; i < info.template_.propertyCount; ++i)
        if (!TK_SetProperty(&properties, info.template_.properties[i].name, info.template_.properties[i].value))
            return TK_cliteral_zero(TK_Properties);
    
    
    return properties;
}




void TK_DestroyProperties(TK_Properties *properties)
{
    if (!properties)
        return;
    
    if (!properties->allocator)
    {
        if (properties->errorQueue)
            TK_enqueue(properties->errorQueue, TK_ERROR_NO_ALLOCATOR);
        
        return;
    }
    
    if (properties->length > properties->capacity)
        if (properties->errorQueue)
            TK_enqueue(properties->errorQueue, TK_StringSliceLiteral("`properties->length > properties->capacity`"));
    
    if (properties->capacity > 0 && !properties->items)
        if (properties->errorQueue)
            TK_enqueue(properties->errorQueue, TK_StringSliceLiteral("`properties->items == NULL` when `properties->capacity > 0`"));
    
    
    if (properties->items)
        TK_free(properties->allocator, properties->items);
    
    properties->capacity = 0;
    properties->length = 0;
}



bool TK_GetProperty(TK_Properties *properties, TK_StringSlice name, TK_Any *value)
{
    if (!properties)
        return false;
    
    if (!properties->allocator)
    {
        if (properties->errorQueue)
            TK_enqueue(properties->errorQueue, TK_ERROR_NO_ALLOCATOR);
        
        return false;
    }
    
    if (properties->length > properties->capacity)
    {
        if (properties->errorQueue)
            TK_enqueue(properties->errorQueue, TK_StringSliceLiteral("`properties->length > properties->capacity`"));
        
        return false;
    }
    
    if (properties->capacity > 0 && !properties->items)
    {
        if (properties->errorQueue)
            TK_enqueue(properties->errorQueue, TK_StringSliceLiteral("`properties->items == NULL` when `properties->capacity > 0`"));
        
        return false;
    }
    
    
    
    for (int item_index = 0; item_index < properties->length; ++item_index)
    {
        TK_Property property = properties->items[item_index];
        
        if (!TK_AreStringSlicesEq(property.name, name))
            continue;
        
        if (value)
            *value = property.value;
        
        return true;
    }
    
    
    
    return false;
}



bool TK_SetProperty(TK_Properties *properties, TK_StringSlice name, TK_Any value)
{
    {
        if (!properties)
            return false;
        
        if (!properties->allocator)
        {
            if (properties->errorQueue)
                TK_enqueue(properties->errorQueue, TK_ERROR_NO_ALLOCATOR);
            
            return false;
        }
        
        if (properties->length > properties->capacity)
        {
            if (properties->errorQueue)
                TK_enqueue(properties->errorQueue, TK_StringSliceLiteral("`properties->length > properties->capacity`"));
            
            return false;
        }
        
        if (properties->capacity > 0 && !properties->items)
        {
            if (properties->errorQueue)
                TK_enqueue(properties->errorQueue, TK_StringSliceLiteral("`properties->items == NULL` when `properties->capacity > 0`"));
            
            return false;
        }
    }
    
    
    
    {
        if (value.type == TK_FACTOR)
            value.data.factor = TK_clamp(value.data.factor, 0.f, 1.f);
        
        if (value.type == TK_BOOL)
            value.data.bool_ = value.data.bool_ ? true : false;
    }
    
    
    
    for (int item_index = 0; item_index < properties->length; ++item_index)
    {
        TK_Property *property = &properties->items[item_index];
        
        if (!TK_AreStringSlicesEq(property->name, name))
            continue;
        
        if (property->strict && value.type != property->value.type)
        {
            if (properties->errorQueue)
                TK_enqueue(properties->errorQueue, TK_StringSliceLiteral("Type mismatch in strict property"));
            
            return false;
        }
        
        property->value = value;
        
        return true;
    }
    
    
    
    if (properties->length == properties->capacity)
    {
        int new_capacity = (properties->capacity + 8) * 2;
        
        if (properties->items)
            properties->items = TK_reallocate(properties->allocator, sizeof(*properties->items) * new_capacity, properties->items);
        else
            properties->items = TK_allocate(properties->allocator, sizeof(*properties->items) * new_capacity);
        
        
        if (!properties->items)
        {
            if (properties->errorQueue)
                TK_enqueue(properties->errorQueue, TK_ERROR_ALLOCATION);
            
            properties->length = 0;
            properties->capacity = 0;
            
            return false;
        }
        
        
        properties->capacity = new_capacity;
    }
    
    
    
    properties->items[properties->length++] = TK_cliteral(TK_Property){
        .name = name,
        .value = value,
    };
    
    
    
    return true;
}
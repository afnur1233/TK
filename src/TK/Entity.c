#include <TK/Entity.h>





void TK_ParentEntity(TK_Entity *child, TK_Entity *parent)
{
    if (!child)
        return;
    
    
    {
        TK_Entity *oldParent = child->parent;
        if (oldParent)
        {
            --oldParent->child_count;
            
            if (oldParent->children == child)
            {
                if (child->next == child)
                    oldParent->children = NULL;
                else
                    oldParent->children = child->next;
            }
            
            child->parent = NULL;
        }
            
        if (child->next)
            child->next->prev = child->prev ? child->prev : NULL;
        
        if (child->prev)
            child->prev->next = child->next ? child->next : NULL;
    }
    
    
    if (parent)
    {
        ++parent->child_count;
        
        child->parent = parent;
        
        if (!parent->children)
        {
            parent->children = child;
            child->next = child;
            child->prev = child;
        }
        else
        {
            child->next = parent->children;
            child->prev = parent->children->prev;
            
            if (child->prev)
                child->prev->next = child;
            
            parent->children->prev = child;
        }
    }
}




TK_Entity *TK_InstanceEntityTemplate(TK_EntityAllocator *allocator, TK_Entity *parent, TK_EntityTemplate template)
{
    {
        if (!allocator)
            return NULL;
        
        if (allocator->capacity > 0 && !allocator->items)
        {
            if (allocator->errorQueue)
                TK_enqueue(allocator->errorQueue, TK_StringSliceLiteral("`allocator->items == 0` when `allocator->capacity`"));
            
            return NULL;
        }
        
        if (allocator->load > allocator->capacity)
        {
            if (allocator->errorQueue)
                TK_enqueue(allocator->errorQueue, TK_StringSliceLiteral("`allocator->load > allocator->capacity`"));
            
            return NULL;
        }
        
        if (allocator->allocator && allocator->load == 0 && allocator->items)
        {
            if (allocator->errorQueue)
                TK_enqueue(allocator->errorQueue, TK_StringSliceLiteral("`allocator->load > allocator->capacity`"));
            
            return NULL;
        }
    }
    
    
    TK_Entity *entity = NULL;
    {
        if (allocator->load == 0)
            for (size_t i = 0; i < allocator->capacity; ++i)
                allocator->items[i].used = false;
        
        if (allocator->load == allocator->capacity)
        {
            if (!allocator->allocator)
            {
                TK_enqueue(allocator->errorQueue, TK_StringSliceLiteral("Ran out of space in entity pool"));
                
                return NULL;
            }
            
            
            size_t new_capacity = (allocator->capacity + 8) * 2;
            
            if (allocator->items)
                allocator->items = TK_reallocate(allocator->allocator, sizeof(*allocator->items) * new_capacity, allocator->items);
            else
                allocator->items = TK_allocate(allocator->allocator, sizeof(*allocator->items) * new_capacity);
            
            
            if (!allocator->items)
            {
                allocator->capacity = 0;
                allocator->load = 0;
                return NULL;
            }
            
            allocator->capacity = new_capacity;
            
            
            for (size_t i = allocator->load; i < new_capacity; ++i)
                allocator->items[i].used = false;
            
            
            entity = &allocator->items[allocator->load++];
            *entity = TK_cliteral(*entity){ .used =  true };
        }
        else
        {
            for (size_t i = 0; i < allocator->capacity; ++i)
            {
                if (allocator->items[i].used)
                    continue;
                
                allocator->load++;
                entity = &allocator->items[i];
                *entity = TK_cliteral(*entity){ .used =  true };
                break;
            }
        }
    }
    
    {
        entity->child_count = 0;
        entity->nickname = template.nickname;
        entity->children = NULL;
        entity->parent = NULL;
        entity->next = NULL;
        entity->prev = NULL;
        TK_ParentEntity(entity, parent);
        
        
        {
            TK_ErrorQueue errorQueue = TK_AllocateQueueOnStack(errorQueue, 10);
            
            
            TK_Properties properties = TK_CreatePropertiesOPT((TK_PropertiesCreateInfo){
                .allocator = allocator->allocator, 
                .errorQueue = &errorQueue, 
                .template_ = template.properties, 
            });
            if (errorQueue.length > 0)
            {
                if (allocator->errorQueue)
                    while (errorQueue.length > 0)
                        TK_enqueue(allocator->errorQueue, TK_dequeue(&errorQueue));
                
                entity->used = false;
                
                return NULL;
            }
            
            entity->properties = properties;
        }
        
        
        for (size_t child_index = 0; child_index < template.children.length; ++child_index)
            if (!TK_InstanceEntityTemplate(allocator, entity, template.children.items[child_index]))
            {
                while (entity->children)
                {
                    TK_ParentEntity(entity->children, NULL);
                    entity->children->used = false;
                }
                
                entity->used = false;
                TK_DestroyProperties(&entity->properties);
                
                return NULL;
            }
    }
    
    
    return entity;
}



void TK_ClearAllEntities(TK_EntityAllocator *allocator)
{
    if (!allocator)
        return;
    
    for (size_t i = 0; i < allocator->capacity; ++i)
        allocator->items[i].used = false;
}



void TK_DestroyEntities(TK_EntityAllocator *allocator)
{
    if (!allocator)
        return;
    
    if (allocator->items)
        TK_free(allocator->allocator, allocator->items);
    
    
    allocator->items = NULL;
    allocator->capacity = 0;
    allocator->load = 0;
}



void TK_DestroyEntity(TK_Entity *entity)
{
    if (!entity)
        return;
    
    TK_ParentEntity(entity, NULL);
    
    TK_Entity *child = entity->children;
    for (size_t i = 0; i < entity->child_count; ++i)
    {
        TK_Entity *next = child->next;
        TK_DestroyEntity(child);
        
        child = next;
    }
    
    entity->used = false;
}
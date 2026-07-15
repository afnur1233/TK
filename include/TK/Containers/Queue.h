#pragma once



#include <TK/Basic.h>





#define TK_Queue(...)                       \
    {                                       \
        struct                              \
        {                                   \
            __typeof__(__VA_ARGS__) *items; \
            size_t capacity;                \
            size_t length;                  \
            size_t read_head;               \
            bool allow_overflow;            \
        };                                  \
    }



#define TK_AllocateQueueOnStack(T, SIZE, ...)                                      \
    (TK_cliteral(T){                                                               \
        .items = TK_cliteral_zero(__typeof__(*TK_cliteral_zero(T).items)[(SIZE)]), \
        .capacity = (SIZE),                                                        \
        __VA_ARGS__                                                                \
    })




#define TK_assert_queue_invariants(...)                             \
    TK_macro_statement(                                             \
        __typeof__(__VA_ARGS__) queue_a __VA_OPT__( = __VA_ARGS__); \
                                                                    \
        TK_assert(queue_a);                                         \
        TK_assert(queue_a->items);                                  \
        TK_assert(queue_a->length <= queue_a->capacity);            \
        TK_assert(queue_a->capacity > 0);                           \
    )




#define TK_dequeue(...)                                        \
    ({                                                         \
        __typeof__(__VA_ARGS__) queue_ = (__VA_ARGS__);        \
        TK_assert_queue_invariants(queue_);                    \
        TK_assert(queue_->length > 0);                         \
                                                               \
        queue_->length--;                                      \
        queue_->items[queue_->read_head++ % queue_->capacity]; \
    })




#define TK_peek_queue(...)                                   \
    ({                                                       \
        __typeof__(__VA_ARGS__) queue_ = (__VA_ARGS__);      \
        TK_assert_queue_invariants(queue_);                  \
                                                             \
                                                             \
        queue_->items[queue_->read_head % queue_->capacity]; \
    })

    


#define TK_enqueue_array(q_, ...)                                                                    \
    TK_macro_statement(                                                                              \
        __typeof__(q_) _queue = (q_);                                                                \
        __typeof__(_queue->items[0]) _args[] = __VA_ARGS__;                                          \
        TK_assert_queue_invariants(_queue);                                                          \
                                                                                                     \
                                                                                                     \
        size_t wrote = 0;                                                                            \
                                                                                                     \
                                                                                                     \
        while (wrote < TK_arraylength(_args))                                                        \
        {                                                                                            \
            if (_queue->length >= _queue->capacity)                                                  \
            {                                                                                        \
                if (!_queue->allow_overflow)                                                         \
                    break;                                                                           \
                else                                                                                 \
                    TK_dequeue(_queue);                                                              \
            }                                                                                        \
                                                                                                     \
                                                                                                     \
            _queue->items[(_queue->read_head + _queue->length) % _queue->capacity] = _args[wrote++]; \
            _queue->length++;                                                                        \
        }                                                                                            \
    )



#define TK_enqueue(q_, ...) TK_enqueue_array(q_, {__VA_ARGS__})
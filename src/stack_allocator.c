#include "stack_allocator.h"

struct s_allocator_s {
    void *memory; 
    void *top;
};

void *s_alloc_init(size_t bytes)
{
    s_allocator_t *allocator = malloc(sizeof(s_allocator_t *));
    allocator->top = allocator->memory = malloc(bytes);
    return allocator;
}

void *s_alloc(s_allocator_t *allocator, size_t bytes, size_t alignment)
{
    void *mem = allocator->top;
    allocator->top += bytes;
    return mem;
}

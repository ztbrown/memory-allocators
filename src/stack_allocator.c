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
    assert((alignment & (alignment - 1)) == 0);
    assert(alignment <= 128);
    assert(alignment >= 1);

    uintptr_t misalignment = ((uintptr_t)allocator->top & (alignment - 1));
    uintptr_t adjustment = alignment - misalignment;
    void *mem = allocator->top + adjustment;

    allocator->top += bytes + adjustment;

    ((char *)mem)[-1] = adjustment;

    return mem;
}

void s_free(s_allocator_t *allocator, void *location)
{
    allocator->top = location - ((char *)location)[-1];
}

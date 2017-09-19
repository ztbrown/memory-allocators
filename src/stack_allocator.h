#ifndef STACK_ALLOCATOR_H 
#define STACK_ALLOCATOR_H

#include <stdlib.h>

typedef struct s_allocator_s s_allocator_t;

void *s_alloc_init(size_t bytes);

void *s_alloc(s_allocator_t *allocator, size_t bytes, size_t alignment);

#endif // STACK_ALLOCATOR_H
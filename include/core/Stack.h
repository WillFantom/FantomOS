#ifndef __STACK_H
#define __STACK_H

#include <core/Common.h>

typedef struct Stack
{
    void *base;
    unsigned int elements;
    unsigned int max_elements;
} stack_t;

void Stack_push(stack_t *stack, uint32_t element);
uint32_t *Stack_pop(stack_t *stack);

#endif

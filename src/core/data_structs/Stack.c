/*  - Stack.c
    AUTHOR: Will Fantom
    DESC:   A data storage type. Fast functions however, data storage can be be expensive
            compared to type such as a bitmap.
*/

#include <core/Stack.h>
#include <core/Common.h>

void Stack_push(stack_t *stack, uint32_t element)
{
    if(stack->elements + 1 <= stack->max_elements)
    {
        *((uint32_t *) (stack->base + (stack->elements * sizeof(void*)))) = (uint32_t) element;
        (stack->elements)++;
    }
}

uint32_t *Stack_pop(stack_t *stack)
{
    if(stack->elements == 0)
        return NULL;

    if(stack->elements - 1 >= 0)
    {
        stack->elements--;
        return (uint32_t) (*((uint32_t *) (stack->base + ((stack->elements) * sizeof(uint32_t)))));
    }
    else
    {
        return NULL;
    }
}

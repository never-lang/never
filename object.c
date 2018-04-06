#include <stdlib.h>
#include <stdio.h>
#include "object.h"

object * object_new(unsigned short size, char * types)
{
    object * ret;
    unsigned short i;
    unsigned int mem_size = sizeof(object) + sizeof(char) * size + sizeof(unsigned int) * size;
    
    ret = (object *)malloc(mem_size);
    if (ret == NULL)
    {
        fprintf(stderr, "cannot allocate an object\n");
        exit(2);
    }
    
    ret->mark = 0;
    ret->size = size;
    
    for (i = 0; i < size; i++)
    {
        ret->types[i] = types[i];
    }
    
    return ret;
}

void object_delete(object * obj)
{
    if (obj == NULL)
    {
        return;
    }
    free(obj);
}

unsigned int object_get_value(object * obj, unsigned int index)
{
    unsigned int * ptr = object_get_ptr(obj, index);
    return *ptr;
}

void object_set_value(object * obj, unsigned int index, unsigned int value)
{
    unsigned int * ptr = object_get_ptr(obj, index);
    *ptr = value;
}

unsigned int * object_get_ptr(object * obj, unsigned int index)
{
    return (unsigned int *)(obj->types + sizeof(char) * obj->size + sizeof(unsigned int) * index);
}



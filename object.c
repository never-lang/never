#include <stdlib.h>
#include <stdio.h>
#include "object.h"

object * object_new()
{
    object * obj = (object *)malloc(sizeof(object));
    
    return obj;
}

object * object_new_int(int value)
{
    object * obj = (object *)malloc(sizeof(object));

    obj->type = OBJECT_INT;
    obj->int_value = value;
    
    return obj;
}

object * object_new_vec(unsigned int size)
{
    object * obj = (object *)malloc(sizeof(object));
    object_vec * vec_value = (object_vec *)malloc(size * sizeof(object_vec));

    vec_value->size = size;

    obj->type = OBJECT_VEC;
    obj->vec_value = vec_value;
    
    return obj;
}

object * object_new_func(unsigned int vec, unsigned int addr)
{
    object * obj = (object *)malloc(sizeof(object));
    object_func * func_value = (object_func *)malloc(sizeof(object_func));

    func_value->vec = vec;
    func_value->addr = addr;

    obj->type = OBJECT_VEC;
    obj->func_value = func_value;

    return obj;
}

void object_delete(object * obj)
{
    switch (obj->type)
    {
        case OBJECT_INT:
        break;
        case OBJECT_VEC:
            free(obj->vec_value);
        break;
        case OBJECT_FUNC:
            free(obj->func_value);
        break;
    }
    free(obj);
}




#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "object.h"

object * object_new()
{
    object * obj = (object *)malloc(sizeof(object));
    
    return obj;
}

object * object_new_float(float value)
{
    object * obj = (object *)malloc(sizeof(object));

    obj->type = OBJECT_FLOAT;
    obj->float_value = value;
    
    return obj;
}

object * object_new_vec(unsigned int size)
{
    object * obj = (object *)malloc(sizeof(object));
    object_vec * vec_value = (object_vec *)malloc(sizeof(object_vec));

    vec_value->size = size;
    if (vec_value->size == 0)
    {
        vec_value->value = NULL;
    }
    else
    {
        vec_value->value = (mem_ptr *)malloc(size * sizeof(mem_ptr));
    }


    obj->type = OBJECT_VEC;
    obj->vec_value = vec_value;
    
    return obj;
}

object * object_new_func(mem_ptr vec, ip_ptr addr)
{
    object * obj = (object *)malloc(sizeof(object));
    object_func * func_value = (object_func *)malloc(sizeof(object_func));

    func_value->vec = vec;
    func_value->addr = addr;

    obj->type = OBJECT_FUNC;
    obj->func_value = func_value;

    return obj;
}

void object_delete(object * obj)
{
    switch (obj->type)
    {
        case OBJECT_UNKNOWN:
            assert(0);
        break;
        case OBJECT_FLOAT:
        break;
        case OBJECT_VEC:
            if (obj->vec_value != NULL && obj->vec_value->value != NULL)
            {
                free(obj->vec_value->value);
            }
            if (obj->vec_value != NULL)
            {
                free(obj->vec_value);
            }
        break;
        case OBJECT_FUNC:
            free(obj->func_value);
        break;
    }
    free(obj);
}

void object_print(object * obj)
{
    switch (obj->type)
    {
        case OBJECT_UNKNOWN:
            printf("object_unknown\n");
        break;
        case OBJECT_FLOAT:
            printf("object_float\n");
        break;
        case OBJECT_VEC:
            printf("object_vec\n");
        break;
        case OBJECT_FUNC:
            printf("object_func\n");
        break;
    }
}


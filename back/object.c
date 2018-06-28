/**
 * Copyright 2018 Slawomir Maludzinski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "object.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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
    case OBJECT_INT:
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
    case OBJECT_INT:
        printf("object_int\n");
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

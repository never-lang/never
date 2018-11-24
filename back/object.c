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
#include <string.h>

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

object * object_new_string(char * value)
{
    object * obj = (object *)malloc(sizeof(object));
    
    obj->type = OBJECT_STRING;
    obj->string_value = strdup(value);
    
    return obj;
}

object * object_new_string_take(char * value)
{
    object * obj = (object *)malloc(sizeof(object));
    
    obj->type = OBJECT_STRING;
    obj->string_value = value;
    
    return obj;
}

object * object_new_string_ref(mem_ptr str_value)
{
    object * obj = (object *)malloc(sizeof(object));
    
    obj->type = OBJECT_STRING_REF;
    obj->string_ref_value = str_value;
    
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

object * object_new_vec_ref(mem_ptr vec_value)
{
    object * obj = (object *)malloc(sizeof(object));
    
    obj->type = OBJECT_VEC_REF;
    obj->vec_ref_value = vec_value;
    
    return obj;
}

object * object_new_arr(unsigned int dims, object_arr_dim * dv)
{
    object * obj = (object *)malloc(sizeof(object));
    object_arr * arr_value = (object_arr *)malloc(sizeof(object_arr));

    arr_value->dims = dims;
    arr_value->dv = dv;
    object_arr_dim_mult(dims, arr_value->dv, &arr_value->elems);

    if (arr_value->elems != 0)
    {
        arr_value->value =
            (mem_ptr *)malloc(arr_value->elems * sizeof(mem_ptr));
    }
    else
    {
        arr_value->value = NULL;
    }

    obj->type = OBJECT_ARRAY;
    obj->arr_value = arr_value;

    return obj;
}

object * object_new_arr_ref(mem_ptr arr_value)
{
    object * obj = (object *)malloc(sizeof(object));
    
    obj->type = OBJECT_ARRAY_REF;
    obj->arr_ref_value = arr_value;
    
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
    case OBJECT_STRING:
        if (obj->string_value != NULL)
        {
            free(obj->string_value);
        }
        break;
    case OBJECT_STRING_REF:
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
    case OBJECT_VEC_REF:
        break;
    case OBJECT_ARRAY:
        if (obj->arr_value != NULL)
        {
            object_arr_delete(obj->arr_value);
        }
        break;
    case OBJECT_ARRAY_REF:
        break;
    case OBJECT_FUNC:
        free(obj->func_value);
        break;
    }
    free(obj);
}

void object_arr_delete(object_arr * arr_value)
{
    if (arr_value->value != NULL)
    {
        free(arr_value->value);
    }
    if (arr_value->dv != NULL)
    {
        object_arr_dim_delete(arr_value->dv);
    }
    free(arr_value);
}

object_arr_dim * object_arr_dim_new(unsigned int dims)
{
    object_arr_dim * dv =
        (object_arr_dim *)malloc(dims * sizeof(object_arr_dim));
    return dv;
}

void object_arr_dim_delete(object_arr_dim * dv) { free(dv); }

void object_arr_dim_mult(unsigned int dims, object_arr_dim * dv,
                         unsigned int * elems)
{
    unsigned int d = 0;
    unsigned int e = 1;

    for (d = 0; d < dims; d++)
    {
        e *= dv[d].elems;
    }
    *elems = e;
    for (d = 0; d < dims; d++)
    {
        if (dv[d].elems != 0)
        {
            e /= dv[d].elems;
            dv[d].mult = e;
        }
        else
        {
            dv[d].mult = 1;
        }
    }
}

unsigned int object_arr_dim_addr(unsigned int dims, object_arr_dim * dv,
                                 object_arr_dim * addr, int * oobounds)
{
    unsigned int m;
    unsigned int addr_int = 0;

    for (m = 0; m < dims; m++)
    {
        if (dv[m].elems <= addr[m].mult)
        {
            *oobounds = m;
            return 0;
        }
        addr_int += dv[m].mult * addr[m].mult;
    }

    *oobounds = -1;
    return addr_int;
}

object_arr_dim * object_arr_dim_copy(unsigned int dims, object_arr_dim * value)
{
    unsigned int d;
    object_arr_dim * dv =
        (object_arr_dim *)malloc(dims * sizeof(object_arr_dim));

    for (d = 0; d < dims; d++)
    {
        dv[d].elems = value[d].elems;
        dv[d].mult = value[d].mult;
    }

    return dv;
}

object * object_arr_copy(object * value)
{
    object * obj = (object *)malloc(sizeof(object));
    object_arr * arr_value = value->arr_value;
    object_arr * copy = (object_arr *)malloc(sizeof(object_arr));

    copy->dims = arr_value->dims;
    copy->elems = arr_value->elems;
    copy->dv = object_arr_dim_copy(arr_value->dims, arr_value->dv);

    if (copy->elems != 0)
    {
        copy->value =
            (mem_ptr *)malloc(copy->elems * sizeof(mem_ptr));
    }
    else
    {
        copy->value = NULL;
    }
    
    obj->type = OBJECT_ARRAY;
    obj->arr_value = copy;
    
    return obj;
}

void object_arr_append(object_arr * arr_value, mem_ptr value)
{
    assert(arr_value->dims == 1);

    arr_value->elems++;
    arr_value->dv[0].elems++;
    
    arr_value->value = (mem_ptr *)realloc(arr_value->value, sizeof(mem_ptr) * arr_value->elems);
    arr_value->value[arr_value->elems - 1] = value;
}

char object_arr_can_add(object_arr * arr1, object_arr * arr2)
{
    unsigned int d;
    if (arr1 == NULL || arr2 == NULL)
    {
        return 0;
    }

    if (arr1->dims != arr2->dims)
    {
        return 0;
    }

    for (d = 0; d < arr1->dims; d++)
    {
        if (arr1->dv[d].elems != arr2->dv[d].elems)
        {
            return 0;
        }
    }

    return 1;
}

char object_arr_can_mult(object_arr * arr1, object_arr * arr2)
{
    if (arr1 == NULL || arr2 == NULL)
    {
        return 0;
    }

    if (arr1->dims != 2 || arr2->dims != 2)
    {
        return 0;
    }
    if (arr1->dv[1].elems == arr2->dv[0].elems)
    {
        return 1;
    }
    return 0;
}

void object_arr_print(object_arr * value)
{
    unsigned int d;

    printf("object_array\n");

    printf("arr->dims %d\n", value->dims);
    printf("arr->elems %d\n", value->elems);
    for (d = 0; d < value->dims; d++)
    {
        printf("arr->dv[%d].elems %d\n", d, value->dv[d].elems);
        printf("arr->dv[%d].mult %d\n", d, value->dv[d].mult);
    }
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
    case OBJECT_STRING:
        printf("object_string\n");
        break;
    case OBJECT_STRING_REF:
        printf("object_string_ref\n");
        break;
    case OBJECT_VEC:
        printf("object_vec\n");
        break;
    case OBJECT_VEC_REF:
        printf("object_vec_ref\n");
        break;
    case OBJECT_ARRAY:
        object_arr_print(obj->arr_value);
        break;
    case OBJECT_ARRAY_REF:
        printf("object_arr_ref\n");
        break;
    case OBJECT_FUNC:
        printf("object_func\n");
        break;
    }
}

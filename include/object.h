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
#ifndef __OBJECT_H__
#define __OBJECT_H__

typedef unsigned int ip_ptr;
typedef unsigned int mem_ptr;
typedef int stack_ptr;

typedef enum object_type
{
    OBJECT_UNKNOWN = 0,
    OBJECT_INT = 1,
    OBJECT_FLOAT = 2,
    OBJECT_ARRAY = 3,
    OBJECT_VEC = 4,
    OBJECT_FUNC = 5
} object_type;

typedef struct object_vec
{
    unsigned int size;
    mem_ptr * value;
} object_vec;

typedef struct object_arr_dim
{
    unsigned int elems;
    unsigned int mult;
} object_arr_dim;

typedef struct object_arr
{
    unsigned int dims;
    unsigned int elems;
    object_arr_dim * dv;
    mem_ptr * value;
} object_arr;

typedef struct object_func
{
    mem_ptr vec;
    ip_ptr addr;
} object_func;

typedef struct object
{
    object_type type;
    union {
        int int_value;            /* OBJECT_INT */
        float float_value;        /* OBJECT_FLOAT */
        object_vec * vec_value;   /* OBJECT_VEC */
        object_arr * arr_value;   /* OBJECT_ARR */
        object_func * func_value; /* OBJECT_FUNC */
    };
} object;

object_arr_dim * object_arr_dim_new(unsigned int dims);
void object_arr_dim_delete(object_arr_dim * dv);

void object_arr_dim_mult(unsigned int dims, object_arr_dim * dv,
                         unsigned int * elems);
unsigned int object_arr_dim_addr(unsigned int dims, object_arr_dim * dv,
                                 object_arr_dim * addr, int * oobounds);

object * object_new_int(int value);
object * object_new_float(float value);
object * object_new_vec(unsigned int size);
object * object_new_arr(unsigned int dims, object_arr_dim * dv);
object * object_new_func(mem_ptr vec, ip_ptr addr);

void object_delete(object * obj);

void object_arr_print(object_arr * value);
void object_print(object * obj);

#endif /* __OBJECT_H__ */

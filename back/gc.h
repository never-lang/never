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
#ifndef __GC_H__
#define __GC_H__

#include "object.h"

typedef enum gc_mem_type
{
    GC_MEM_UNKNOWN = 0,
    GC_MEM_IP = 1,
    GC_MEM_ADDR = 2,
    GC_MEM_STACK = 3,
} gc_mem_type;

typedef struct gc_mem
{
    char mark;
    object * object_value;
    mem_ptr next;
} gc_mem;

typedef struct gc_stack
{
    gc_mem_type type;
    union {
        ip_ptr ip;
        mem_ptr addr;
        stack_ptr sp;
    };
} gc_stack;

typedef struct gc
{
    mem_ptr free;
    unsigned int mem_size;
    gc_mem * mem;
    unsigned int w_index;
    unsigned int wb_top[2];
    mem_ptr * wb_list[2];
} gc;

gc * gc_new(unsigned int mem_size);
void gc_delete(gc * collector);

void gc_sweep_all(gc * collector);
void gc_mark_vec(gc * collector, mem_ptr addr);
void gc_mark_arr(gc * collector, mem_ptr addr);
void gc_mark(gc * collector, mem_ptr addr);
void gc_mark_access(gc * collector, gc_stack * omfalos, int stack_size);
void gc_run_omfalos(gc * collector, gc_stack * omfalos, int stack_size);

void gc_run(gc * collector, gc_stack * omfalos, int stack_size,
            mem_ptr global_vec);

mem_ptr gc_alloc_any(gc * collector, object * value);
mem_ptr gc_alloc_int(gc * collector, int value);
mem_ptr gc_alloc_long(gc * collector, long long value);
mem_ptr gc_alloc_float(gc * collector, float value);
mem_ptr gc_alloc_double(gc * collector, double value);
mem_ptr gc_alloc_char(gc * collector, char value);
mem_ptr gc_alloc_string(gc * collector, char * value);
mem_ptr gc_alloc_string_take(gc * collector, char * value);
mem_ptr gc_alloc_string_ref(gc * collector, mem_ptr str);
mem_ptr gc_alloc_c_ptr(gc * collector, void * value);
mem_ptr gc_alloc_vec(gc * collector, unsigned int size);
mem_ptr gc_alloc_vec_ref(gc * collector, mem_ptr vec);
mem_ptr gc_alloc_arr(gc * collector, unsigned int dims, object_arr_dim * dv);
mem_ptr gc_alloc_arr_ref(gc * collector, mem_ptr array);
mem_ptr gc_alloc_func(gc * collector, mem_ptr vec, ip_ptr addr);

mem_ptr gc_copy_arr(gc * collector, mem_ptr addr);

int gc_get_int(gc * collector, mem_ptr addr);
void gc_set_int(gc * collector, mem_ptr addr, int value);
void gc_inc_int(gc * collector, mem_ptr addr);
void gc_dec_int(gc * collector, mem_ptr addr);
int * gc_get_int_ptr(gc * collector, mem_ptr addr);

long long gc_get_long(gc * collector, mem_ptr addr);
void gc_set_long(gc * collector, mem_ptr addr, long long value);
long long * gc_get_long_ptr(gc * collector, mem_ptr addr);

float gc_get_float(gc * collector, mem_ptr addr);
void gc_set_float(gc * collector, mem_ptr addr, float value);
float * gc_get_float_ptr(gc * collector, mem_ptr addr);

double gc_get_double(gc * collector, mem_ptr addr);
void gc_set_double(gc * collector, mem_ptr addr, double value);
double * gc_get_double_ptr(gc * collector, mem_ptr addr);

char gc_get_char(gc * collector, mem_ptr addr);
void gc_set_char(gc * collector, mem_ptr addr, char value);
char * gc_get_char_ptr(gc * collector, mem_ptr addr);

char * gc_get_string(gc * collector, mem_ptr addr);
void gc_set_string(gc * collector, mem_ptr addr, char * value);
char ** gc_get_string_ptr(gc * collector, mem_ptr addr);
unsigned int gc_get_string_len(gc * collector, mem_ptr addr);

mem_ptr gc_get_string_ref(gc * collector, mem_ptr addr);
void gc_set_string_ref(gc * collector, mem_ptr addr, mem_ptr string_ref);

void * gc_get_c_ptr(gc * collector, mem_ptr addr);
void ** gc_get_c_ptr_ptr(gc * collector, mem_ptr addr);
void gc_set_c_ptr(gc * collector, mem_ptr addr, void * value);

mem_ptr gc_get_vec(gc * collector, mem_ptr addr, unsigned int vec_index);
void gc_set_vec(gc * collector, mem_ptr addr, unsigned int vec_index,
                mem_ptr value);
unsigned int gc_get_vec_size(gc * collector, mem_ptr addr);
mem_ptr gc_get_vec_ref(gc * collector, mem_ptr addr);
mem_ptr gc_set_vec_ref(gc * collector, mem_ptr addr, mem_ptr vec_ref);

mem_ptr gc_get_arr(gc * collector, mem_ptr addr);
mem_ptr gc_get_arr_ref(gc * collector, mem_ptr addr);
void gc_set_arr_ref(gc * collector, mem_ptr addr, mem_ptr array);

object_arr * gc_get_arr_obj(gc * collector, mem_ptr addr);
unsigned int gc_get_arr_dim_elems(gc * collector, mem_ptr addr,
                                  unsigned int dim);
unsigned int gc_get_arr_dims(gc * collector, mem_ptr addr);
unsigned int gc_get_arr_elems(gc * collector, mem_ptr addr);
object_arr_dim * gc_get_arr_dv(gc * collector, mem_ptr addr);

mem_ptr gc_get_arr_elem(gc * collector, mem_ptr addr, unsigned int elem_index);
void gc_set_arr_elem(gc * collector, mem_ptr addr, unsigned int elem_index,
                     mem_ptr value);
void gc_append_arr_elem(gc * collector, mem_ptr addr, mem_ptr value);

ip_ptr gc_get_func_addr(gc * collector, mem_ptr func_addr);
mem_ptr gc_get_func_vec(gc * collector, mem_ptr func_addr);
void gc_set_func_addr(gc * collector, mem_ptr func_addr, ip_ptr addr);
void gc_set_func_vec(gc * collector, mem_ptr func_addr, mem_ptr vec);

object * gc_get_object(gc * collector, mem_ptr addr);

gc_stack * gc_stack_new(int stack_size);
void gc_stack_delete(gc_stack * stack);
void gc_stack_print(gc_stack * stack, int stack_size);

void gc_object_print(gc * collector, mem_ptr addr);

#endif /* __GC_H__ */

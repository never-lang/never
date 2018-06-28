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
} gc;

gc * gc_new(unsigned int mem_size);
void gc_delete(gc * collector);

void gc_mark_all(gc * collector);
void gc_sweep_all(gc * collector);
void gc_mark_vec(gc * collector, mem_ptr addr);
void gc_mark(gc * collector, mem_ptr addr);
void gc_mark_access(gc * collector, gc_stack * omfalos, int stack_size);
void gc_run_omfalos(gc * collector, gc_stack * omfalos, int stack_size);

void gc_run(gc * collector, gc_stack * omfalos, int stack_size,
            mem_ptr global_vec);

mem_ptr gc_alloc_any(gc * collector, object * value);
mem_ptr gc_alloc_int(gc * collector, int value);
mem_ptr gc_alloc_float(gc * collector, float value);
mem_ptr gc_alloc_vec(gc * collector, unsigned int size);
mem_ptr gc_alloc_func(gc * collector, mem_ptr vec, ip_ptr addr);

int gc_get_int(gc * collector, mem_ptr addr);
void gc_set_int(gc * collector, mem_ptr addr, int value);

float gc_get_float(gc * collector, mem_ptr addr);
void gc_set_float(gc * collector, mem_ptr addr, float value);

mem_ptr gc_get_vec(gc * collector, mem_ptr addr, unsigned int vec_index);
void gc_set_vec(gc * collector, mem_ptr addr, unsigned int vec_index,
                mem_ptr value);

ip_ptr gc_get_func_addr(gc * collector, mem_ptr func_addr);
mem_ptr gc_get_func_vec(gc * collector, mem_ptr func_addr);
void gc_set_func_addr(gc * collector, mem_ptr func_addr, ip_ptr addr);
void gc_set_func_vec(gc * collector, mem_ptr func_addr, mem_ptr vec);

object gc_get_object(gc * collector, mem_ptr addr);

gc_stack * gc_stack_new(int stack_size);
void gc_stack_delete(gc_stack * stack);
void gc_stack_print(gc_stack * stack, int stack_size);

#endif /* __GC_H__ */

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
#include "gc.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

gc * gc_new(unsigned int mem_size)
{
    unsigned int i;

    gc * collector = (gc *)malloc(sizeof(gc));
    gc_mem * mem = (gc_mem *)malloc(mem_size * sizeof(gc_mem));
    mem_ptr * wb_list_0 = (mem_ptr *)malloc(mem_size * sizeof(mem_ptr));
    mem_ptr * wb_list_1 = (mem_ptr *)malloc(mem_size * sizeof(mem_ptr));

    mem[0].object_value = NULL;
    mem[0].next = 0;
    mem[0].mark = 0;
    for (i = 1; i < mem_size; i++)
    {
        mem[i].object_value = NULL;
        mem[i].next = i + 1;
        mem[i].mark = 0;
    }
    mem[mem_size - 1].object_value = NULL;
    mem[mem_size - 1].next = 0;
    mem[mem_size - 1].mark = 0;

    collector->w_index = 0;
    collector->wb_top[0] = 0;
    collector->wb_top[1] = 0;
    collector->wb_list[0] = wb_list_0;
    collector->wb_list[1] = wb_list_1;

    collector->free = 1;
    collector->mem_size = mem_size;
    collector->mem = mem;

    return collector;
}

void gc_delete(gc * collector)
{
    unsigned int i;

    for (i = 0; i < collector->mem_size; i++)
    {
        if (collector->mem[i].object_value != NULL)
        {
            object_delete(collector->mem[i].object_value);
        }
    }
    if (collector->mem != NULL)
    {
        free(collector->mem);
    }
    if (collector->wb_list[0] != NULL)
    {
        free(collector->wb_list[0]);
    }
    if (collector->wb_list[1] != NULL)
    {
        free(collector->wb_list[1]);
    }
    free(collector);
}

void gc_sweep_all(gc * collector)
{
    unsigned int i;
    unsigned int w_index = collector->w_index;
    unsigned int b_index = (collector->w_index + 1) % 2;

    for (i = 0; i < collector->wb_top[w_index]; i++)
    {
        mem_ptr idx = collector->wb_list[w_index][i];

        if (collector->mem[idx].mark == 0 &&
            collector->mem[idx].object_value != NULL)
        {
            object_delete(collector->mem[idx].object_value);
            collector->mem[idx].object_value = NULL;

            collector->mem[idx].next = collector->free;
            collector->free = idx;
        }
        else if (collector->mem[idx].mark == 1)
        {
            collector->mem[idx].mark = 0;
            collector->wb_list[b_index][collector->wb_top[b_index]++] = idx;
        }
    }

    collector->wb_top[w_index] = 0;
    collector->w_index = b_index;
}

void gc_mark_vec(gc * collector, mem_ptr addr)
{
    unsigned int i;
    object_vec * vec;

    if (addr == 0)
    {
        return;
    }

    if (collector->mem[addr].mark == 1)
    {
        return;
    }

    collector->mem[addr].mark = 1;

    vec = collector->mem[addr].object_value->vec_value;
    for (i = 0; i < vec->size; i++)
    {
        gc_mark(collector, vec->value[i]);
    }
}

void gc_mark_arr(gc * collector, mem_ptr addr)
{
    unsigned int e;
    object_arr * array;

    if (addr == 0)
    {
        return;
    }

    if (collector->mem[addr].mark == 1)
    {
        return;
    }

    collector->mem[addr].mark = 1;

    array = collector->mem[addr].object_value->arr_value;
    for (e = 0; e < array->elems; e++)
    {
        gc_mark(collector, array->value[e]);
    }
}

void gc_mark(gc * collector, mem_ptr addr)
{
    if (addr == 0)
    {
        return;
    }

    if (collector->mem[addr].object_value != NULL)
    {
        switch (collector->mem[addr].object_value->type)
        {
        case OBJECT_UNKNOWN:
            assert(0);
            break;
        case OBJECT_INT:
            collector->mem[addr].mark = 1;
            break;
        case OBJECT_LONG:
            collector->mem[addr].mark = 1;
            break;
        case OBJECT_FLOAT:
            collector->mem[addr].mark = 1;
            break;
        case OBJECT_DOUBLE:
            collector->mem[addr].mark = 1;
            break;
        case OBJECT_CHAR:
            collector->mem[addr].mark = 1;
            break;
        case OBJECT_STRING:
            collector->mem[addr].mark = 1;
            break;
        case OBJECT_STRING_REF:
            collector->mem[addr].mark = 1;
            gc_mark(collector,
                    collector->mem[addr].object_value->string_ref_value);
            break;
        case OBJECT_C_PTR:
            collector->mem[addr].mark = 1;
            break;
        case OBJECT_VEC:
            gc_mark_vec(collector, addr);
            break;
        case OBJECT_VEC_REF:
            collector->mem[addr].mark = 1;
            gc_mark_vec(collector,
                        collector->mem[addr].object_value->vec_ref_value);
            break;
        case OBJECT_ARRAY:
            gc_mark_arr(collector, addr);
            break;
        case OBJECT_ARRAY_REF:
            collector->mem[addr].mark = 1;
            gc_mark_arr(collector,
                        collector->mem[addr].object_value->arr_ref_value);
            break;
        case OBJECT_FUNC:
            collector->mem[addr].mark = 1;
            gc_mark_vec(collector,
                        collector->mem[addr].object_value->func_value->vec);
            break;
        }
    }
}

void gc_mark_access(gc * collector, gc_stack * omfalos, int stack_size)
{
    int i = 0;

    for (i = 0; i < stack_size; i++)
    {
        if (omfalos[i].type == GC_MEM_ADDR && omfalos[i].addr > 0 &&
            collector->mem[omfalos[i].addr].mark == 0)
        {
            gc_mark(collector, omfalos[i].addr);
        }
    }
}

void gc_run_omfalos(gc * collector, gc_stack * omfalos, int stack_size)
{
    gc_mark_access(collector, omfalos, stack_size);
    gc_sweep_all(collector);
}

void gc_run(gc * collector, gc_stack * stack, int stack_size,
            mem_ptr global_vec)
{
    if (collector->wb_top[collector->w_index] < collector->mem_size * 0.8)
    {
        return;
    }

    gc_mark_access(collector, stack, stack_size);
    if (global_vec > 0)
    {
        gc_mark(collector, global_vec);
    }
    gc_sweep_all(collector);
}

mem_ptr gc_alloc_any(gc * collector, object * value)
{
    mem_ptr loc = collector->free;
    unsigned int w_index = collector->w_index;

    if (loc == 0)
    {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }

    collector->mem[loc].object_value = value;
    collector->free = collector->mem[loc].next;

    collector->wb_list[w_index][collector->wb_top[w_index]++] = loc;

    return loc;
}

mem_ptr gc_alloc_int(gc * collector, int value)
{
    return gc_alloc_any(collector, object_new_int(value));
}

mem_ptr gc_alloc_long(gc * collector, long long value)
{
    return gc_alloc_any(collector, object_new_long(value));
}

mem_ptr gc_alloc_float(gc * collector, float value)
{
    return gc_alloc_any(collector, object_new_float(value));
}

mem_ptr gc_alloc_double(gc * collector, double value)
{
    return gc_alloc_any(collector, object_new_double(value));
}

mem_ptr gc_alloc_char(gc * collector, char value)
{
    return gc_alloc_any(collector, object_new_char(value));
}

mem_ptr gc_alloc_string(gc * collector, char * value)
{
    return gc_alloc_any(collector, object_new_string(value));
}

mem_ptr gc_alloc_string_take(gc * collector, char * value)
{
    return gc_alloc_any(collector, object_new_string_take(value));
}

mem_ptr gc_alloc_string_ref(gc * collector, mem_ptr str)
{
    return gc_alloc_any(collector, object_new_string_ref(str));
}

mem_ptr gc_alloc_c_ptr(gc * collector, void * value)
{
    return gc_alloc_any(collector, object_new_c_ptr(value));
}

mem_ptr gc_alloc_vec(gc * collector, unsigned int size)
{
    return gc_alloc_any(collector, object_new_vec(size));
}

mem_ptr gc_alloc_vec_ref(gc * collector, mem_ptr vec)
{
    return gc_alloc_any(collector, object_new_vec_ref(vec));
}

mem_ptr gc_alloc_arr(gc * collector, unsigned int dims, object_arr_dim * dv)
{
    return gc_alloc_any(collector, object_new_arr(dims, dv));
}

mem_ptr gc_alloc_arr_ref(gc * collector, mem_ptr array)
{
    return gc_alloc_any(collector, object_new_arr_ref(array));
}

mem_ptr gc_alloc_func(gc * collector, mem_ptr vec, ip_ptr addr)
{
    return gc_alloc_any(collector, object_new_func(vec, addr));
}

mem_ptr gc_copy_arr(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_ARRAY);

    return gc_alloc_any(collector,
                        object_arr_copy(collector->mem[addr].object_value));
}

int gc_get_int(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_INT);

    return collector->mem[addr].object_value->int_value;
}

void gc_set_int(gc * collector, mem_ptr addr, int value)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_INT);

    collector->mem[addr].object_value->int_value = value;
}

void gc_inc_int(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_INT);

    collector->mem[addr].object_value->int_value++;
}

void gc_dec_int(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_INT);

    collector->mem[addr].object_value->int_value--;
}

int * gc_get_int_ptr(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_INT);
    
    return &collector->mem[addr].object_value->int_value;
}
long long gc_get_long(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_LONG);

    return collector->mem[addr].object_value->long_value;
}

void gc_set_long(gc * collector, mem_ptr addr, long long value)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_LONG);

    collector->mem[addr].object_value->long_value = value;
}

long long * gc_get_long_ptr(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_LONG);
    
    return &collector->mem[addr].object_value->long_value;
}

float gc_get_float(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_FLOAT);

    return collector->mem[addr].object_value->float_value;
}

void gc_set_float(gc * collector, mem_ptr addr, float value)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_FLOAT);

    collector->mem[addr].object_value->float_value = value;
}

float * gc_get_float_ptr(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_FLOAT);
    
    return &collector->mem[addr].object_value->float_value;
}

double gc_get_double(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_DOUBLE);

    return collector->mem[addr].object_value->double_value;
}

void gc_set_double(gc * collector, mem_ptr addr, double value)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_DOUBLE);

    collector->mem[addr].object_value->double_value = value;
}

double * gc_get_double_ptr(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_DOUBLE);
    
    return &collector->mem[addr].object_value->double_value;
}

char gc_get_char(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_CHAR);
    
    return collector->mem[addr].object_value->char_value;
}

void gc_set_char(gc * collector, mem_ptr addr, char value)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_CHAR);
    
    collector->mem[addr].object_value->char_value = value;
}

char * gc_get_char_ptr(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_CHAR);
    
    return &collector->mem[addr].object_value->char_value;
}

char * gc_get_string(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_STRING);
    
    return collector->mem[addr].object_value->string_value;
}

void gc_set_string(gc * collector, mem_ptr addr, char * value)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_STRING);
    
    if (collector->mem[addr].object_value->string_value != NULL)
    {
        free(collector->mem[addr].object_value->string_value);
    }
    collector->mem[addr].object_value->string_value = strdup(value);
}

char ** gc_get_string_ptr(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_STRING);
    
    return &collector->mem[addr].object_value->string_value;
}

unsigned int gc_get_string_len(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_STRING);

    return strlen(collector->mem[addr].object_value->string_value);
}

mem_ptr gc_get_string_ref(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_STRING_REF);
    
    return collector->mem[addr].object_value->string_ref_value;
}

void gc_set_string_ref(gc * collector, mem_ptr addr, mem_ptr string_ref)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_STRING_REF);
    
    collector->mem[addr].object_value->string_ref_value = string_ref;
}

void * gc_get_c_ptr(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_C_PTR);
    
    return collector->mem[addr].object_value->c_ptr_value;
}

void ** gc_get_c_ptr_ptr(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_C_PTR);
    
    return &collector->mem[addr].object_value->c_ptr_value;
}

void gc_set_c_ptr(gc * collector, mem_ptr addr, void * value)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_C_PTR);

    collector->mem[addr].object_value->c_ptr_value = value;
}

mem_ptr gc_get_vec(gc * collector, mem_ptr addr, unsigned int vec_index)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_VEC);
    assert(collector->mem[addr].object_value->vec_value->size >= vec_index);

    return collector->mem[addr].object_value->vec_value->value[vec_index];
}

void gc_set_vec(gc * collector, mem_ptr addr, unsigned int vec_index,
                mem_ptr value)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_VEC);
    assert(collector->mem[addr].object_value->vec_value->size >= vec_index);

    collector->mem[addr].object_value->vec_value->value[vec_index] = value;
}

unsigned int gc_get_vec_size(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_VEC);
    
    return collector->mem[addr].object_value->vec_value->size;
}

mem_ptr gc_get_vec_ref(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_VEC_REF);
    
    return collector->mem[addr].object_value->vec_ref_value;
}

mem_ptr gc_set_vec_ref(gc * collector, mem_ptr addr, mem_ptr vec_ref)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_VEC_REF);
    
    return collector->mem[addr].object_value->vec_ref_value = vec_ref;
}

mem_ptr gc_get_arr(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_ARRAY);

    return addr;
}

mem_ptr gc_get_arr_ref(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_ARRAY_REF);
    
    return collector->mem[addr].object_value->arr_ref_value;
}

void gc_set_arr_ref(gc * collector, mem_ptr addr, mem_ptr array)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_ARRAY_REF);
    
    collector->mem[addr].object_value->arr_ref_value = array;
}

object_arr * gc_get_arr_obj(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_ARRAY);

    return collector->mem[addr].object_value->arr_value;
}

unsigned int gc_get_arr_dim_elems(gc * collector, mem_ptr addr,
                                  unsigned int dim)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_ARRAY);
    assert(collector->mem[addr].object_value->arr_value->dims > dim);

    return collector->mem[addr].object_value->arr_value->dv[dim].elems;
}

unsigned int gc_get_arr_dims(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_ARRAY);

    return collector->mem[addr].object_value->arr_value->dims;
}

unsigned int gc_get_arr_elems(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_ARRAY);

    return collector->mem[addr].object_value->arr_value->elems;
}

object_arr_dim * gc_get_arr_dv(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_ARRAY);

    return collector->mem[addr].object_value->arr_value->dv;
}

mem_ptr gc_get_arr_elem(gc * collector, mem_ptr addr, unsigned int elem_index)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_ARRAY);
    assert(collector->mem[addr].object_value->arr_value->elems >= elem_index);

    return collector->mem[addr].object_value->arr_value->value[elem_index];
}

void gc_set_arr_elem(gc * collector, mem_ptr addr, unsigned int elem_index,
                     mem_ptr value)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_ARRAY);
    assert(collector->mem[addr].object_value->arr_value->elems >= elem_index);

    collector->mem[addr].object_value->arr_value->value[elem_index] = value;
}

void gc_append_arr_elem(gc * collector, mem_ptr addr, mem_ptr value)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_ARRAY);
    
    object_arr_append(collector->mem[addr].object_value->arr_value, value);
}

ip_ptr gc_get_func_addr(gc * collector, mem_ptr func_addr)
{
    assert(collector->mem_size >= func_addr);
    assert(collector->mem[func_addr].object_value->type == OBJECT_FUNC);

    return collector->mem[func_addr].object_value->func_value->addr;
}

mem_ptr gc_get_func_vec(gc * collector, mem_ptr func_addr)
{
    assert(collector->mem_size >= func_addr);
    assert(collector->mem[func_addr].object_value->type == OBJECT_FUNC);

    return collector->mem[func_addr].object_value->func_value->vec;
}

void gc_set_func_addr(gc * collector, mem_ptr func_addr, ip_ptr addr)
{
    assert(collector->mem_size >= func_addr);
    assert(collector->mem[func_addr].object_value->type == OBJECT_FUNC);

    collector->mem[func_addr].object_value->func_value->addr = addr;
}

void gc_set_func_vec(gc * collector, mem_ptr func_addr, mem_ptr vec)
{
    assert(collector->mem_size >= func_addr);
    assert(collector->mem[func_addr].object_value->type == OBJECT_FUNC);

    collector->mem[func_addr].object_value->func_value->vec = vec;
}

object * gc_get_object(gc * collector, mem_ptr addr)
{
    assert(collector->mem_size >= addr);

    return collector->mem[addr].object_value;
}

gc_stack * gc_stack_new(int stack_size)
{
    gc_stack * stack = (gc_stack *)malloc(stack_size * sizeof(gc_stack));

    return stack;
}

void gc_stack_delete(gc_stack * stack) { free(stack); }

void gc_stack_print(gc_stack * stack, int stack_size)
{
    int sp = 0;
    assert(stack_size >= sp);

    for (sp = 0; sp <= stack_size; sp++)
    {
        switch (stack[sp].type)
        {
        case GC_MEM_UNKNOWN:
            assert(0);
            break;
        case GC_MEM_IP:
            printf("ip: %u\n", stack[sp].ip);
            break;
        case GC_MEM_ADDR:
            printf("addr: %d\n", stack[sp].addr);
            break;
        case GC_MEM_STACK:
            printf("sp: %d\n", stack[sp].sp);
            break;
        }
    }
}

void gc_object_print(gc * collector, mem_ptr addr)
{
    printf("mem_size %u\n", collector->mem_size);
    printf("addr %u\n", addr);

    object_print(collector->mem[addr].object_value);
}

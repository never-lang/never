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
    union
    {
        ip_ptr ip;
        mem_ptr addr;
        stack_ptr sp; 
    };
} gc_stack;

typedef struct gc
{
    mem_ptr free;
    int mem_size;
    int stack_size;
    gc_mem * mem;
    gc_stack * stack;
} gc;

gc * gc_new(int mem_size, int stack_size);
void gc_delete(gc * collector);

void gc_mark_all(gc * collector);
void gc_sweep_all(gc * collector);
void gc_mark_vec(gc * collector, mem_ptr addr);
void gc_mark(gc * collector, mem_ptr addr);
void gc_mark_access(gc * collector, gc_stack * omfalos, int stack_size);
void gc_run_omfalos(gc * collector, gc_stack * omfalos, int stack_size);

void gc_run(gc * collector, int stack_size, mem_ptr global_vec);

mem_ptr gc_alloc_any(gc * collector, object * value);
mem_ptr gc_alloc_int(gc * collector, int value);
mem_ptr gc_alloc_vec(gc * collector, unsigned int size);
mem_ptr gc_alloc_func(gc * collector, mem_ptr vec, ip_ptr addr);

int gc_get_int(gc * collector, mem_ptr addr);
void gc_set_int(gc * collector, mem_ptr addr, int value);

mem_ptr gc_get_vec(gc * collector, mem_ptr addr, unsigned int vec_index);
void gc_set_vec(gc * collector, mem_ptr addr, unsigned int vec_index, mem_ptr value);

ip_ptr gc_get_func_addr(gc * collector, mem_ptr func_addr);
mem_ptr gc_get_func_vec(gc * collector, mem_ptr func_addr);
void gc_set_func_addr(gc * collector, mem_ptr func_addr, ip_ptr addr);
void gc_set_func_vec(gc * collector, mem_ptr func_addr, mem_ptr vec);

void gc_stack_print(gc * collector, int sp);

#endif /* __GC_H__ */



#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "gc.h"

gc * gc_new(unsigned int mem_size)
{
    unsigned int i;

    gc * collector = (gc *)malloc(sizeof(gc));
    gc_mem * mem = (gc_mem *)malloc(mem_size * sizeof(gc_mem));
    
    mem[0].object_value = NULL;
    mem[0].next = 0;
    for (i = 1; i < mem_size; i++)
    {
        mem[i].object_value = NULL;
        mem[i].next = i + 1;
    }
    mem[mem_size - 1].object_value = NULL;
    mem[mem_size - 1].next = 0;
        
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
    free(collector);
}

void gc_mark_all(gc * collector)
{
    unsigned int i;
    
    for (i = 0; i < collector->mem_size; i++)
    {
        collector->mem[i].mark = 0;
    }
}

void gc_sweep_all(gc * collector)
{
    unsigned int i;
    
    for (i = 0; i < collector->mem_size; i++)
    {
        if (collector->mem[i].mark == 0 && collector->mem[i].object_value != NULL)
        {
            object_delete(collector->mem[i].object_value);
            collector->mem[i].object_value = NULL;

            collector->mem[i].next = collector->free;
            collector->free = i;
        }
    }
}

void gc_mark_vec(gc * collector, mem_ptr addr)
{
    unsigned int i;
    object_vec * vec;
    
    if (collector->mem[addr].mark == 1)
    {
        return;
    }

    vec = collector->mem[addr].object_value->vec_value;
    for (i = 0; i < vec->size; i++)
    {
        gc_mark(collector, vec->value[i]);
    }

    collector->mem[addr].mark = 1;
}

void gc_mark(gc * collector, mem_ptr addr)
{
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
            case OBJECT_VEC:
                gc_mark_vec(collector, addr);
            break;
            case OBJECT_FUNC:
                gc_mark_vec(collector, collector->mem[addr].object_value->func_value->vec);
                collector->mem[addr].mark = 1;
            break;
        }
    }
}

void gc_mark_access(gc * collector, gc_stack * omfalos, int stack_size)
{
    int i = 0;

    for (i = 0; i < stack_size; i++)
    {
        if (omfalos[i].type == GC_MEM_ADDR &&
            omfalos[i].addr > 0 && 
            collector->mem[omfalos[i].addr].mark == 0)
        {
            gc_mark(collector, omfalos[i].addr);
        }
    }
}

void gc_run_omfalos(gc * collector, gc_stack * omfalos, int stack_size)
{
    gc_mark_all(collector);
    gc_mark_access(collector, omfalos, stack_size);
    gc_sweep_all(collector);
}

void gc_run(gc * collector, gc_stack * stack, int stack_size, mem_ptr global_vec)
{
    gc_mark_all(collector);
    gc_mark_access(collector, stack, stack_size);
    if (global_vec > 0)
    {
        gc_mark_vec(collector, global_vec);
    }
    gc_sweep_all(collector);
}

mem_ptr gc_alloc_any(gc * collector, object * value)
{
    mem_ptr loc = collector->free;

    if (loc == 0)
    {
        printf("out of memory\n");
        exit(1);
    }

    collector->mem[loc].object_value = value;
    collector->free = collector->mem[loc].next;

    return loc;
}

mem_ptr gc_alloc_int(gc * collector, int value)
{
    return gc_alloc_any(collector, object_new_int(value));
}

mem_ptr gc_alloc_vec(gc * collector, unsigned int size)
{
    return gc_alloc_any(collector, object_new_vec(size));
}

mem_ptr gc_alloc_func(gc * collector, mem_ptr vec, ip_ptr addr)
{
    return gc_alloc_any(collector, object_new_func(vec, addr));
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

mem_ptr gc_get_vec(gc * collector, mem_ptr addr, unsigned int vec_index)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_VEC);
    assert(collector->mem[addr].object_value->vec_value->size >= vec_index);

    return collector->mem[addr].object_value->vec_value->value[vec_index];
}

void gc_set_vec(gc * collector, mem_ptr addr, unsigned int vec_index, mem_ptr value)
{
    assert(collector->mem_size >= addr);
    assert(collector->mem[addr].object_value->type == OBJECT_VEC);
    assert(collector->mem[addr].object_value->vec_value->size >= vec_index);

    collector->mem[addr].object_value->vec_value->value[vec_index] = value;
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

gc_stack * gc_stack_new(int stack_size)
{
    gc_stack * stack = (gc_stack *)malloc(stack_size * sizeof(gc_stack));

    return stack;
}

void gc_stack_delete(gc_stack * stack)
{
    free(stack);
}

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
                printf("addr: %d", stack[sp].addr);
            break;
            case GC_MEM_STACK:
                printf("sp: %d\n", stack[sp].sp);
            break;
        }
    }    
}


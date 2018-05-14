#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "gc.h"

gc * gc_new(unsigned int mem_size)
{
    unsigned int i;

    gc * collector = (gc *)malloc(sizeof(gc));
    gc_mem * mem = (gc_mem *)malloc(mem_size * sizeof(gc_mem));
    
    for (i = 0; i < mem_size; i++)
    {
        mem[i].object_value = NULL;
        mem[i].next = i + 1;
    }
    
    collector->free = 0;
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
        if (collector->mem[i].mark == 0)
        {
            object_delete(collector->mem[i].object_value);
            collector->mem[i].object_value = NULL;

            collector->mem[i].next = collector->free;
            collector->free = i;
        }
    }
}

void gc_mark_vec(gc * collector, unsigned int index)
{
    unsigned int i;
    object_vec * vec;
    
    if (collector->mem[index].mark == 1)
    {
        return;
    }

    collector->mem[index].mark = 1;

    vec = (object_vec *)collector->mem[index].object_value;
    for (i = 0; i < vec->size; i++)
    {
        gc_mark(collector, vec->value[i]);
    }
}

void gc_mark(gc * collector, unsigned int index)
{
    if (collector->mem[index].object_value != NULL)
    {
        switch (collector->mem[index].object_value->type)
        {
            case OBJECT_INT:
                collector->mem[index].mark = 1;
            break;
            case OBJECT_VEC:
                gc_mark_vec(collector, index);
            break;
            case OBJECT_FUNC:
                collector->mem[index].mark = 1;
            break;
        }
    }
}

void gc_mark_access(gc * collector, unsigned int * omfalos, unsigned int size)
{
    unsigned int i = 0;

    for (i = 0; i < size; i++)
    {
        if (collector->mem[omfalos[i]].mark == 0)
        {
            gc_mark(collector, omfalos[i]);
        }
    }
}

void gc_run(gc * collector, unsigned int * omfalos, unsigned int size)
{
    gc_mark_all(collector);
    gc_mark_access(collector, omfalos, size);
    gc_sweep_all(collector);
}

unsigned int gc_alloc_any(gc * collector, object * value)
{
    unsigned int loc = collector->free;

    if (loc >= collector->mem_size)
    {
        printf("out of memory\n");
        exit(1);
    }

    collector->mem[loc].object_value = value;
    collector->free = collector->mem[loc].next;

    return loc;
}

unsigned int gc_alloc_int(gc * collector, int value)
{
    return gc_alloc_any(collector, object_new_int(value));
}

unsigned int gc_alloc_vec(gc * collector, unsigned int size)
{
    return gc_alloc_any(collector, object_new_vec(size));
}

unsigned int gc_alloc_func(gc * collector, unsigned int vec, unsigned int addr)
{
    return gc_alloc_any(collector, object_new_func(vec, addr));
}

int gc_get_int(gc * collector, unsigned int index)
{
    assert(collector->mem_size >= index);
    assert(collector->mem[index].object_value->type == OBJECT_INT);    

    return collector->mem[index].object_value->int_value;
}

void gc_set_int(gc * collector, unsigned int index, int value)
{
    assert(collector->mem_size >= index);
    assert(collector->mem[index].object_value->type == OBJECT_INT);    

    collector->mem[index].object_value->int_value = value;
}

unsigned int gc_get_vec(gc * collector, unsigned int index, unsigned int vec_index)
{
    assert(collector->mem_size >= index);
    assert(collector->mem[index].object_value->type == OBJECT_VEC);
    assert(collector->mem[index].object_value->vec_value->size >= vec_index);

    return collector->mem[index].object_value->vec_value->value[vec_index];
}

void gc_set_vec(gc * collector, unsigned int index, unsigned int vec_index, unsigned int value)
{
    assert(collector->mem_size >= index);
    assert(collector->mem[index].object_value->type == OBJECT_VEC);
    assert(collector->mem[index].object_value->vec_value->size >= vec_index);

    collector->mem[index].object_value->vec_value->value[vec_index] = value;
}

unsigned int gc_get_func_addr(gc * collector, unsigned int index)
{
    assert(collector->mem_size >= index);
    assert(collector->mem[index].object_value->type == OBJECT_FUNC);    

    return collector->mem[index].object_value->func_value->addr;
}

unsigned int gc_get_func_vec(gc * collector, unsigned int index)
{
    assert(collector->mem_size >= index);
    assert(collector->mem[index].object_value->type == OBJECT_FUNC);    

    return collector->mem[index].object_value->func_value->vec;
}

void gc_set_func_addr(gc * collector, unsigned int index, unsigned int addr)
{
    assert(collector->mem_size >= index);
    assert(collector->mem[index].object_value->type == OBJECT_FUNC);    

    collector->mem[index].object_value->func_value->addr = addr;
}

void gc_set_func_vec(gc * collector, unsigned int index, unsigned int vec)
{
    assert(collector->mem_size >= index);
    assert(collector->mem[index].object_value->type == OBJECT_FUNC);    

    collector->mem[index].object_value->func_value->vec = vec;
}



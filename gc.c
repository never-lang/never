#include <stdlib.h>
#include <stdio.h>
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

unsigned int gc_alloc_int(gc * collector, int value)
{
    unsigned int loc = collector->free;

    if (loc >= collector->mem_size)
    {
        printf("out of memory\n");
        exit(1);
    }

    collector->mem[loc].object_value = object_new_int(value);
    collector->free = collector->mem[loc].next;

    return loc;
}

unsigned int gc_alloc_vec(gc * collector, unsigned int size)
{
    unsigned int loc = collector->free;

    if (loc >= collector->mem_size)
    {
        printf("out of memory\n");
        exit(1);
    }

    collector->mem[loc].object_value = object_new_vec(size);
    collector->free = collector->mem[loc].next;

    return loc;
}

unsigned int gc_alloc_func(gc * collector, unsigned int vec, unsigned int addr)
{
    unsigned int loc = collector->free;

    if (loc >= collector->mem_size)
    {
        printf("out of memory\n");
        exit(1);
    }

    collector->mem[loc].object_value = object_new_func(vec, addr);
    collector->free = collector->mem[loc].next;

    return loc;
}




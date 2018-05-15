#include <stdio.h>
#include <assert.h>
#include "gc.h"

void test_one()
{
    gc * collector = gc_new(100);    
    gc_delete(collector);
}

void test_two()
{
    unsigned int int1;
    unsigned int int2;
    unsigned int vec1;
    gc * collector = gc_new(100);
    
    int1 = gc_alloc_int(collector, -1);
    int2 = gc_alloc_int(collector, -2);
    vec1 = gc_alloc_vec(collector, 2);
    
    gc_set_int(collector, int1, 12);
    gc_set_int(collector, int2, 14);
    
    gc_set_vec(collector, vec1, 0, int1);
    gc_set_vec(collector, vec1, 1, int2);

    assert(gc_get_int(collector, int1) == 12);
    assert(gc_get_int(collector, int2) == 14);
    assert(gc_get_vec(collector, vec1, 0) == int1);
    assert(gc_get_vec(collector, vec1, 1) == int2);
    
    gc_delete(collector);
}

void test_three()
{
    unsigned int func1;
    unsigned int func2;
    unsigned int vec1;
    gc * collector = gc_new(100);
    
    func1 = gc_alloc_func(collector, -12, -13);
    func2 = gc_alloc_func(collector, -14, -15);
    vec1 = gc_alloc_vec(collector, 2);
    
    gc_set_func_vec(collector, func1, 12);
    gc_set_func_vec(collector, func2, 14);
    gc_set_func_addr(collector, func1, 13);
    gc_set_func_addr(collector, func2, 15);
    
    gc_set_vec(collector, vec1, 0, func1);
    gc_set_vec(collector, vec1, 1, func2);

    assert(gc_get_func_vec(collector, func1) == 12);
    assert(gc_get_func_vec(collector, func2) == 14);
    assert(gc_get_func_addr(collector, func1) == 13);
    assert(gc_get_func_addr(collector, func2) == 15);
    assert(gc_get_vec(collector, vec1, 0) == func1);
    assert(gc_get_vec(collector, vec1, 1) == func2);
    
    gc_delete(collector);
}

void test_four()
{
    unsigned int int1;
    unsigned int int2;
    unsigned int int3;
    unsigned int int4;
    unsigned int vec1;
    unsigned int omfalos[2];
    gc * collector = gc_new(100);
    
    int1 = gc_alloc_int(collector, 12);
    int2 = gc_alloc_int(collector, 14);
    int3 = gc_alloc_int(collector, -15);
    int4 = gc_alloc_int(collector, -16);
    vec1 = gc_alloc_vec(collector, 2);
    
    gc_set_int(collector, int3, -gc_get_int(collector, int3));
    gc_set_int(collector, int4, -gc_get_int(collector, int4));
    
    gc_set_vec(collector, vec1, 0, int1);
    gc_set_vec(collector, vec1, 1, int2);

    omfalos[0] = vec1;
    omfalos[1] = int1;
    
    gc_run(collector, omfalos, 2);

    assert(gc_get_int(collector, int1) == 12);
    assert(gc_get_int(collector, int2) == 14);
    assert(gc_get_vec(collector, vec1, 0) == int1);
    assert(gc_get_vec(collector, vec1, 1) == int2);
    
    gc_delete(collector);
}

void test_five()
{
    static const unsigned int val_size = 90;
    static const unsigned int mem_size = 100;
    unsigned int i, j;
    unsigned int ints[val_size];
    gc * collector = gc_new(mem_size);
    
    for (i = 0; i < mem_size; i++)
    {
        for (j = 0; j < val_size; j++)
        {
            ints[j] = gc_alloc_int(collector, -1);
            
            gc_set_int(collector, ints[j], (int)(i * mem_size + j));
            assert(gc_get_int(collector, ints[j]) == (int)(i * mem_size + j));
        }
        gc_run(collector, NULL, 0);
    }

    gc_delete(collector);
}

int main(int argc, char * argv[])
{
    test_one();
    test_two();
    test_three();
    test_four();
    test_five();

    return 0;
}


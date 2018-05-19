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
#include <stdio.h>
#include <assert.h>
#include "gc.h"

void test_one()
{
    gc * collector = gc_new(1000);
    gc_delete(collector);
}

void test_two()
{
    mem_ptr float1;
    mem_ptr float2;
    mem_ptr vec1;
    gc * collector = gc_new(100);
    
    float1 = gc_alloc_float(collector, -1);
    float2 = gc_alloc_float(collector, -2);
    vec1 = gc_alloc_vec(collector, 2);
    
    gc_set_float(collector, float1, 12);
    gc_set_float(collector, float2, 14);
    
    gc_set_vec(collector, vec1, 0, float1);
    gc_set_vec(collector, vec1, 1, float2);

    assert(gc_get_float(collector, float1) == 12);
    assert(gc_get_float(collector, float2) == 14);
    assert(gc_get_vec(collector, vec1, 0) == float1);
    assert(gc_get_vec(collector, vec1, 1) == float2);
    
    gc_delete(collector);
}

void test_three()
{
    mem_ptr func1;
    mem_ptr func2;
    mem_ptr vec1;
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
    mem_ptr float1;
    mem_ptr float2;
    mem_ptr float3;
    mem_ptr float4;
    mem_ptr vec1;
    gc_stack omfalos[2];
    gc * collector = gc_new(100);
    
    float1 = gc_alloc_float(collector, 12);
    float2 = gc_alloc_float(collector, 14);
    float3 = gc_alloc_float(collector, -15);
    float4 = gc_alloc_float(collector, -16);
    vec1 = gc_alloc_vec(collector, 2);
    
    gc_set_float(collector, float3, -gc_get_float(collector, float3));
    gc_set_float(collector, float4, -gc_get_float(collector, float4));
    
    gc_set_vec(collector, vec1, 0, float1);
    gc_set_vec(collector, vec1, 1, float2);

    omfalos[0].type = GC_MEM_ADDR;
    omfalos[0].addr = vec1;
    omfalos[1].type = GC_MEM_ADDR;
    omfalos[1].addr = float1;
    
    gc_run_omfalos(collector, omfalos, 2);

    assert(gc_get_float(collector, float1) == 12);
    assert(gc_get_float(collector, float2) == 14);
    assert(gc_get_vec(collector, vec1, 0) == float1);
    assert(gc_get_vec(collector, vec1, 1) == float2);
    
    gc_delete(collector);
}

void test_five()
{
    static const unsigned int val_size = 90;
    static const unsigned int mem_size = 100;
    unsigned int i, j;
    gc_stack ints[val_size];
    gc * collector = gc_new(mem_size);
    
    for (i = 0; i < mem_size; i++)
    {
        for (j = 0; j < val_size; j++)
        {
            ints[j].type = GC_MEM_ADDR;
            ints[j].addr = gc_alloc_float(collector, -1);
            
            gc_set_float(collector, ints[j].addr, (float)(i * mem_size + j));
            assert(gc_get_float(collector, ints[j].addr) == (float)(i * mem_size + j));
        }
        gc_run_omfalos(collector, NULL, 0);
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


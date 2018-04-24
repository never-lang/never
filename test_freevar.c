#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "freevar.h"

void mix(int * values, int size)
{
    int j;
    
    for (j = 0; j < 8; j++)
    {
        int f, t, v;
        
        f = rand() % size;
        t = rand() % size;
        
        v = values[f];
        values[f] = values[t];
        values[t] = v;
    }
}

void test_table(int * values, int size)
{
    int i;
    btree * tree = NULL;
    
    for (i = 0; i < size; i++)
    {
        btree_insert(&tree, values[i]);
    }
    
    mix(values, size);

    for (i = 0; i < size / 2; i++)
    {
        btree_delete(&tree, values[i]);
    }
    for (i = 0; i < size / 2; i++)
    {
        assert(btree_search(tree, values[i]) == 0);
    }
    for (i = size / 2; i < size; i++)
    {
        assert(btree_search(tree, values[i]) == 1);
    }
    
    btree_destroy(&tree);
}

void test_one()
{
    int i; 
    int size = 12;
    int values[] = { 1, 2, 12, 10, 11, 13, 19, 17, 22, 29, 65, 21 };
    
    for (i = 0; i < 100; i++)
    {
        mix(values, 12);
        test_table(values, size);       
    }
}

int main(int argc, char * argv[])
{
    test_one();

    return 0;
}


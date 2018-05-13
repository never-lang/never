#include <stdio.h>
#include "gc.h"

void test_one()
{
    gc * collector = gc_new(100);
    
    gc_delete(collector);
}

int main(int argc, char * argv[])
{
    test_one();

    return 0;
}


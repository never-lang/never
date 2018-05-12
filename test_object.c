#include <stdio.h>
#include <assert.h>
#include "object.h"

void test_one()
{
    object * obj1 = object_new();
        
    object_delete(obj1);
}

int main(int argc, char * argv[])
{
    test_one();
    
    return 0;
}



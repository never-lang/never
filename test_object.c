#include <stdio.h>
#include <assert.h>
#include "object.h"

void test_one()
{
    object * obj1 = object_new_int(100);
        
    object_delete(obj1);
}

void test_two()
{
    object * obj1 = object_new_vec(20);
    
    object_delete(obj1);
}

void test_three()
{
    object * obj1 = object_new_func(10, 30);
    
    object_delete(obj1);
}

int main(int argc, char * argv[])
{
    test_one();
    test_two();
    test_three();
    
    return 0;
}



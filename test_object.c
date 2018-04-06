#include <stdio.h>
#include <assert.h>
#include "object.h"

void test_one()
{
    char values[] = { TYPE_UINT, TYPE_UINT, TYPE_UINT, TYPE_UINT };

    object * obj1 = object_new(4, values);
    
    object_set_value(obj1, 0, 10);
    object_set_value(obj1, 3, 20);
    
    assert(object_get_value(obj1, 0) == 10);
    assert(object_get_value(obj1, 3) == 20);
    
    object_delete(obj1);
}

int main(int argc, char * argv[])
{
    test_one();
    
    return 0;
}



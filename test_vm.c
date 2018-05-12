#include <stdio.h>
#include "vm.h"

void test_one()
{
    vm * never = vm_new();
    
    vm_delete(never);
}

int main(int argc, char * argv[])
{
    test_one();

    return 0;
}


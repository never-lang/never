#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

vm * vm_new()
{
    vm * never = (vm *)malloc(sizeof(vm));

    never->gp = 0;
    never->fp = 0;
    never->sp = 0;
    never->ip = 0;
    
    return never;
}

void vm_delete(vm * never)
{
    free(never);
}


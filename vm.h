#ifndef __VM_H__
#define __VM_H__

#include "bytecode.h"

typedef struct vm
{
    int gp;
    int fp;
    int sp;
    int ip;
} vm;

vm * vm_new();
void vm_delete(vm * never);

void vm_execute(bytecode * code, unsigned int size);

#endif /* __VM_H__ */


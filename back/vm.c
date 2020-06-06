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
#include "vm.h"
#include "vmexec.h"
#include "bytecode.h"
#include "gc.h"
#include "dlcache.h"
#include <stdlib.h>
#include <stdio.h>

vm * vm_new(unsigned int mem_size, unsigned int stack_size)
{
    vm * machine = (vm *)malloc(sizeof(vm));

    machine->sp = -1;
    machine->fp = -1;
    machine->pp = -1;
    machine->gp = 0;
    machine->ip = 0;

    machine->stack_size = stack_size;
    machine->stack = gc_stack_new(stack_size);
    machine->collector = gc_new(mem_size);
    machine->dlib_cache = dlcache_new(DEFAULT_DLCACHE_SIZE);
    machine->exception = EXCEPT_NO_UNKNOWN;
    machine->line_no = 0;
    machine->initialized = 0;

    vm_execute_op_test();

    return machine;
}

void vm_delete(vm * machine)
{
    if (machine->stack != NULL)
    {
        gc_stack_delete(machine->stack);
    }
    if (machine->collector != NULL)
    {
        gc_delete(machine->collector);
    }
    if (machine->dlib_cache != NULL)
    {
        dlcache_delete(machine->dlib_cache);
    }
    free(machine);
}

void vm_print_stack_trace(vm * machine)
{
    ip_ptr ip = { 0 };
    ip_ptr line = { 0 };
    stack_ptr fp = { 0 };

    fp = machine->fp;
    while (fp > 0)
    {
        line = machine->stack[fp - 3].ip;
        ip = machine->stack[fp].ip;
        printf("called from line %u ip:%u fp:%d\n", line, ip, fp);

        fp = machine->stack[fp - 1].sp;
    }
}

void vm_print(vm * machine, const char * msg)
{
    printf("machine:\n");
    printf("\tsp: %d\n", machine->sp);
    printf("\tfp: %d\n", machine->fp);
    printf("\tgp: %u\n", machine->gp);
    printf("\tip: %u\n", machine->ip);
    printf("\texcept: %d\n", machine->exception);
    printf("\tline_no: %u\n", machine->line_no);
    printf("\tstack_size: %u\n", machine->stack_size);
    printf("\tmem_size: %u\n", machine->collector->mem_size);
    printf("\trunning: %d\n", machine->running);
    printf("\tmessage: %s\n", msg);
    printf("\n");
}


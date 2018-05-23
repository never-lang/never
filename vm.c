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
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <fenv.h>
#include "vm.h"
#include "gc.h"
#include "utils.h"
#include "libmath.h"

vm_execute_str vm_execute_op[] = {
    { BYTECODE_UNKNOWN, vm_execute_unknown },
    { BYTECODE_FLOAT, vm_execute_float },
    { BYTECODE_ID_LOCAL, vm_execute_id_local },
    { BYTECODE_ID_GLOBAL, vm_execute_id_global },
    { BYTECODE_ID_FUNC_FUNC, vm_execute_id_func_func },
    { BYTECODE_ID_FUNC_ADDR, vm_execute_id_func_addr },
    { BYTECODE_JUMPZ, vm_execute_jumpz },
    { BYTECODE_JUMP, vm_execute_jump },
    { BYTECODE_LABEL, vm_execute_label },
    { BYTECODE_OP_NEG, vm_execute_op_neg },
    { BYTECODE_OP_ADD, vm_execute_op_add },
    { BYTECODE_OP_SUB, vm_execute_op_sub },
    { BYTECODE_OP_MUL, vm_execute_op_mul },
    { BYTECODE_OP_DIV, vm_execute_op_div },
    { BYTECODE_OP_LT, vm_execute_op_lt },
    { BYTECODE_OP_GT, vm_execute_op_gt },
    { BYTECODE_OP_LTE, vm_execute_op_lte },
    { BYTECODE_OP_GTE, vm_execute_op_gte },
    { BYTECODE_OP_EQ, vm_execute_op_eq },
    { BYTECODE_OP_NEQ, vm_execute_op_neq },
    { BYTECODE_FUNC_DEF, vm_execute_func_def },
    { BYTECODE_GLOBAL_VEC, vm_execute_global_vec },
    { BYTECODE_MARK, vm_execute_mark },
    { BYTECODE_CALL, vm_execute_call },
    { BYTECODE_RET, vm_execute_ret },
    { BYTECODE_LINE, vm_execute_line },
    { BYTECODE_BUILD_IN, vm_execute_build_in },
    { BYTECODE_HALT, vm_execute_halt }
};

static void vm_execute_op_test()
{
    unsigned int i = 0;
    for (i = 0; i < BYTECODE_END; i++)
    {
        assert(i == vm_execute_op[i].type);
    }
}

void vm_check_stack(vm * machine)
{
    if (machine->sp >= machine->stack_size)
    {
        fprintf(stderr, "stack too large\n");
        vm_print(machine);
        exit(1);
    }
}

void vm_execute_unknown(vm * machine, bytecode * code)
{
    fprintf(stderr, "unknown bytecode\n");
    assert(0);
}

void vm_execute_float(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr addr = gc_alloc_float(machine->collector, code->real.value);
    
    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}

void vm_execute_id_local(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr addr = machine->stack[machine->sp - (code->id_local.stack_level - code->id_local.index)].addr;
    
    machine->sp++;
    vm_check_stack(machine);
    
    entry.type = GC_MEM_ADDR;
    entry.addr = addr;
    
    machine->stack[machine->sp] = entry;
}

void vm_execute_id_global(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr addr = gc_get_vec(machine->collector, machine->gp, code->id_global.index);

    machine->sp++;
    vm_check_stack(machine);
    
    entry.type = GC_MEM_ADDR;
    entry.addr = addr;
    
    machine->stack[machine->sp] = entry;
}

void vm_execute_id_func_func(vm * machine, bytecode * code)
{
    fprintf(stderr, "at this stage id_func_func should be set to id_func_addr with bytecode_func_addr\n");
    assert(0);
}

void vm_execute_id_func_addr(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr vec = machine->stack[machine->sp].addr;
    mem_ptr addr = gc_alloc_func(machine->collector, vec, code->id_func.func_addr);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}

void vm_execute_jumpz(vm * machine, bytecode * code)
{
    float a = gc_get_float(machine->collector, machine->stack[machine->sp].addr);

    if (a == 0)
    {
         machine->ip = machine->ip + code->jump.offset;
    }
    machine->sp--;
}

void vm_execute_jump(vm * machine, bytecode * code)
{
    machine->ip = machine->ip + code->jump.offset;
}

void vm_execute_label(vm * machine, bytecode * code)
{
    /* no op */
}

/* a op b
 * a
 * b
 */
void vm_execute_op_neg(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_float(machine->collector, -a);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry; 
}

void vm_execute_op_add(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    float b = gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr addr = gc_alloc_float(machine->collector, a + b);
    
    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_sub(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    float b = gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr addr = gc_alloc_float(machine->collector, a - b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry; 
    machine->sp--;
}

void vm_execute_op_mul(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    float b = gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr addr = gc_alloc_float(machine->collector, a * b);
    
    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_div(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    float b = gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr addr = 0;

    if (b == 0)
    {
        print_error_msg(machine->line_no, "cannot divide by zero\n");
        machine->running = VM_ERROR;
        return;
    }

    addr = gc_alloc_float(machine->collector, a / b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_lt(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    float b = gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr addr = gc_alloc_float(machine->collector, a < b);
    
    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_gt(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    float b = gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr addr = gc_alloc_float(machine->collector, a > b);
    
    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_lte(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    float b = gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr addr = gc_alloc_float(machine->collector, a >= b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_gte(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    float b = gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr addr = gc_alloc_float(machine->collector, a <= b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_eq(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    float b = gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr addr = gc_alloc_float(machine->collector, a == b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_neq(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    float b = gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr addr = gc_alloc_float(machine->collector, a != b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_func_def(vm * machine, bytecode * code)
{
    /* no op */
}

void vm_execute_global_vec(vm * machine, bytecode * code)
{
    int c;
    gc_stack entry = { 0 };
    mem_ptr addr = gc_alloc_vec(machine->collector, code->global_vec.count);
    
    for (c = code->global_vec.count - 1; c >= 0; c--)
    {
        gc_set_vec(machine->collector, addr, c, machine->stack[machine->sp--].addr);
    }
    
    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;
    
    machine->stack[machine->sp] = entry;
}

void vm_execute_mark(vm * machine, bytecode * code)
{
    gc_stack entry1 = { 0 };
    gc_stack entry2 = { 0 };
    gc_stack entry3 = { 0 };
    
    entry1.type = GC_MEM_ADDR;
    entry1.addr = machine->gp;
    
    entry2.type = GC_MEM_STACK;
    entry2.addr = machine->fp;
    
    entry3.type = GC_MEM_IP;
    entry3.ip = code->mark.addr;

    machine->stack[machine->sp + 1] = entry1;
    machine->stack[machine->sp + 2] = entry2;
    machine->stack[machine->sp + 3] = entry3;
    
    machine->fp = machine->sp = machine->sp + 3;
    vm_check_stack(machine);
}

void vm_execute_call(vm * machine, bytecode * code)
{
    mem_ptr gp = gc_get_func_vec(machine->collector, machine->stack[machine->sp].addr);
    ip_ptr ip = gc_get_func_addr(machine->collector, machine->stack[machine->sp].addr);

    machine->gp = gp;
    machine->ip = ip;
    machine->sp--;
}

void vm_execute_ret(vm * machine, bytecode * code)
{
    machine->gp = machine->stack[machine->fp - 2].addr;
    machine->ip = machine->stack[machine->fp].ip;
    machine->stack[machine->fp - 2] = machine->stack[machine->sp];
    machine->sp = machine->fp - 2;
    machine->fp = machine->stack[machine->fp - 1].sp;

    gc_run(machine->collector, machine->stack, machine->sp + 1, machine->gp);
}

void vm_execute_line(vm * machine, bytecode * code)
{
    machine->line_no = code->line.no;
}

void vm_execute_build_in(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float x = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    float value = 0;

    feclearexcept(FE_ALL_EXCEPT);

    switch (code->build_in.id)
    {
        case LIB_MATH_UNKNOWN:
            fprintf(stderr, "unknown math function\n");
            assert(0);
        break;
        case LIB_MATH_SIN:
            value = sinf(x);
        break;
        case LIB_MATH_COS:
            value = cosf(x);
        break;
        case LIB_MATH_TAN:
            value = tanf(x);
        break;
        case LIB_MATH_EXP:
            value = expf(x);
        break;
        case LIB_MATH_LOG:
            value = logf(x);
        break;
        case LIB_MATH_SQRT:
            value = sqrtf(x);
        break;
    }

    if (fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW))
    {
        print_error_msg(machine->line_no, "an error occurred in the mathematical function\n");
        machine->running = VM_ERROR;
        return;        
    }

    mem_ptr addr = gc_alloc_float(machine->collector, value);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry; 
}

void vm_execute_halt(vm * machine, bytecode * code)
{
    machine->running = VM_HALT;
}

int vm_execute(vm * machine, bytecode * code, unsigned int size, float * ret)
{
    bytecode * bc = NULL;
    
    machine->running = VM_RUNNING;
    while (machine->running == VM_RUNNING)
    {
        bc = code + machine->ip;
        machine->ip++;
        vm_execute_op[bc->type].execute(machine, bc);
    }
    
    if (machine->running == VM_HALT)
    {
        *ret = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
        return 0;
    }

    return 1;
}

vm * vm_new(unsigned int mem_size, unsigned int stack_size)
{
    vm * machine = (vm *)malloc(sizeof(vm));

    machine->sp = -1;
    machine->fp = -1;
    machine->gp = 0;
    machine->ip = 0;

    machine->stack_size = stack_size;
    machine->stack = gc_stack_new(stack_size);
    machine->collector = gc_new(mem_size);

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
    free(machine);
}

void vm_print(vm * machine)
{
    printf("machine:\n");
    printf("\tsp: %d\n", machine->sp);
    printf("\tfp: %d\n", machine->fp);
    printf("\tgp: %u\n", machine->gp);
    printf("\tip: %u\n", machine->ip);
    printf("\tline_no: %u\n", machine->line_no);
    printf("\tstack_size: %u\n", machine->stack_size);
    printf("\tmem_size: %u\n", machine->collector->mem_size);
    printf("\trunning: %d\n", machine->running);
    printf("\n");
}



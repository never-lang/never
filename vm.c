#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "vm.h"

vm_execute_str vm_execute_op[] = {
    { BYTECODE_UNKNOWN, vm_execute_unknown },
    { BYTECODE_INT, vm_execute_int },
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
    { BYTECODE_FUNC_DEF, vm_execute_func_def },
    { BYTECODE_GLOBAL_VEC, vm_execute_global_vec },
    { BYTECODE_MARK, vm_execute_mark },
    { BYTECODE_CALL, vm_execute_call },
    { BYTECODE_RET, vm_execute_ret },
    { BYTECODE_LINE, vm_execute_line }
};

static void vm_execute_op_test()
{
    unsigned int i = 0;
    for (i = 0; i < BYTECODE_END; i++)
    {
        assert(i == vm_execute_op[i].type);
    }
}

void vm_execute_unknown(vm * machine, bytecode * code)
{
}

void vm_execute_int(vm * machine, bytecode * code)
{
}

void vm_execute_id_local(vm * machine, bytecode * code)
{
}

void vm_execute_id_global(vm * machine, bytecode * code)
{
}

void vm_execute_id_func_func(vm * machine, bytecode * code)
{
}

void vm_execute_id_func_addr(vm * machine, bytecode * code)
{
}

void vm_execute_jumpz(vm * machine, bytecode * code)
{
}

void vm_execute_jump(vm * machine, bytecode * code)
{
}

void vm_execute_label(vm * machine, bytecode * code)
{
}

void vm_execute_op_neg(vm * machine, bytecode * code)
{
}

void vm_execute_op_add(vm * machine, bytecode * code)
{
}

void vm_execute_op_sub(vm * machine, bytecode * code)
{
}

void vm_execute_op_mul(vm * machine, bytecode * code)
{
}

void vm_execute_op_div(vm * machine, bytecode * code)
{
}

void vm_execute_op_lt(vm * machine, bytecode * code)
{
}

void vm_execute_op_gt(vm * machine, bytecode * code)
{
}

void vm_execute_op_lte(vm * machine, bytecode * code)
{
}

void vm_execute_op_gte(vm * machine, bytecode * code)
{
}

void vm_execute_op_eq(vm * machine, bytecode * code)
{
}

void vm_execute_func_def(vm * machine, bytecode * code)
{
}

void vm_execute_global_vec(vm * machine, bytecode * code)
{
}

void vm_execute_mark(vm * machine, bytecode * code)
{
}

void vm_execute_call(vm * machine, bytecode * code)
{
}

void vm_execute_ret(vm * machine, bytecode * code)
{
}

void vm_execute_line(vm * machine, bytecode * code)
{
}

void vm_execute(bytecode * code, unsigned int size)
{
}

vm * vm_new()
{
    vm * machine = (vm *)malloc(sizeof(vm));

    machine->gp = 0;
    machine->fp = 0;
    machine->sp = 0;
    machine->ip = 0;

    vm_execute_op_test();
        
    return machine;
}

void vm_delete(vm * machine)
{
    free(machine);
}


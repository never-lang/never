#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "vm.h"
#include "gc.h"

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
    { BYTECODE_LINE, vm_execute_line },
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
        printf("stack too large\n");
        vm_print(machine);
        exit(1);
    }
}

void vm_execute_unknown(vm * machine, bytecode * code)
{
    printf("unknown bytecode\n");
    assert(0);
}

void vm_execute_int(vm * machine, bytecode * code)
{
    unsigned int addr = gc_alloc_int(machine->collector, code->integer.value);
    
    machine->sp++;
    vm_check_stack(machine);

    machine->stack[machine->sp] = addr;

    /* printf("%d:%s\n", code->addr, __func__); */
}

void vm_execute_id_local(vm * machine, bytecode * code)
{
    unsigned int addr = machine->stack[machine->sp - (code->id_local.stack_level - code->id_local.index)];
    
    machine->sp++;
    vm_check_stack(machine);
    
    machine->stack[machine->sp] = addr;
    
    /* printf("%d:%s sp=%d sl=%d ind=%d\n", code->addr, __func__,
                                         machine->sp, code->id_local.stack_level,
                                         code->id_local.index); */
}

void vm_execute_id_global(vm * machine, bytecode * code)
{
    unsigned int addr = gc_get_vec(machine->collector, machine->gp, code->id_global.index);

    machine->sp++;
    vm_check_stack(machine);
    
    machine->stack[machine->sp] = addr;

    /* printf("%d:%s\n", code->addr, __func__); */
}

void vm_execute_id_func_func(vm * machine, bytecode * code)
{
    printf("at this stage id_func_func should be set to id_func_addr with bytecode_func_addr\n");
    assert(0);
}

void vm_execute_id_func_addr(vm * machine, bytecode * code)
{
    unsigned int vec = machine->stack[machine->sp];
    unsigned int addr = gc_alloc_func(machine->collector, vec, code->id_func.func_addr);

    machine->stack[machine->sp] = addr;
    
    /* printf("%d:%s\n", code->addr, __func__); */
}

void vm_execute_jumpz(vm * machine, bytecode * code)
{
    int a = gc_get_int(machine->collector, machine->stack[machine->sp]);

    if (a == 0)
    {
         machine->ip = machine->ip + code->jump.offset;
    }
    machine->sp--;
    
    /* printf("%d:%s a=%d\n", code->addr, __func__, a); */
}

void vm_execute_jump(vm * machine, bytecode * code)
{
    machine->ip = machine->ip + code->jump.offset;
    
    /* printf("%d:%s\n", code->addr, __func__); */
}

void vm_execute_label(vm * machine, bytecode * code)
{
    /* no op */
    /* printf("%d:%s\n", code->addr, __func__); */
}

/* a op b
 * a
 * b
 */
void vm_execute_op_neg(vm * machine, bytecode * code)
{
    int a = gc_get_int(machine->collector, machine->stack[machine->sp]);

    machine->stack[machine->sp] = gc_alloc_int(machine->collector, -a);

    /* printf("%d:%s\n", code->addr, __func__); */
}

void vm_execute_op_add(vm * machine, bytecode * code)
{
    int a = gc_get_int(machine->collector, machine->stack[machine->sp]);
    int b = gc_get_int(machine->collector, machine->stack[machine->sp - 1]);

    machine->stack[machine->sp - 1] = gc_alloc_int(machine->collector, a + b);
    machine->sp--;
    
    /* printf("%d:%s\n", code->addr, __func__); */
}

void vm_execute_op_sub(vm * machine, bytecode * code)
{
    int a = gc_get_int(machine->collector, machine->stack[machine->sp]);
    int b = gc_get_int(machine->collector, machine->stack[machine->sp - 1]);

    machine->stack[machine->sp - 1] = gc_alloc_int(machine->collector, a - b);
    machine->sp--;
    
    /* printf("%d:%s\n", code->addr, __func__); */
}

void vm_execute_op_mul(vm * machine, bytecode * code)
{
    int a = gc_get_int(machine->collector, machine->stack[machine->sp]);
    int b = gc_get_int(machine->collector, machine->stack[machine->sp - 1]);

    machine->stack[machine->sp - 1] = gc_alloc_int(machine->collector, a * b);
    machine->sp--;
    
    /* printf("%d:%s\n", code->addr, __func__); */
}

void vm_execute_op_div(vm * machine, bytecode * code)
{
    int a = gc_get_int(machine->collector, machine->stack[machine->sp]);
    int b = gc_get_int(machine->collector, machine->stack[machine->sp - 1]);

    if (b == 0)
    {
        printf("cannot divide by zero at line %d\n", machine->line_no);
        vm_print(machine);
        exit(1);
    }

    machine->stack[machine->sp - 1] = gc_alloc_int(machine->collector, a / b);
    machine->sp--;
    
    /* printf("%d:%s\n", code->addr, __func__); */
}

void vm_execute_op_lt(vm * machine, bytecode * code)
{
    int a = gc_get_int(machine->collector, machine->stack[machine->sp]);
    int b = gc_get_int(machine->collector, machine->stack[machine->sp - 1]);

    machine->stack[machine->sp - 1] = gc_alloc_int(machine->collector, a < b);
    machine->sp--;
    
    /* printf("%d:%s\n", code->addr, __func__); */
}

void vm_execute_op_gt(vm * machine, bytecode * code)
{
    int a = gc_get_int(machine->collector, machine->stack[machine->sp]);
    int b = gc_get_int(machine->collector, machine->stack[machine->sp - 1]);

    machine->stack[machine->sp - 1] = gc_alloc_int(machine->collector, a > b);
    machine->sp--;
    
    /* printf("%d:%s\n", code->addr, __func__); */
}

void vm_execute_op_lte(vm * machine, bytecode * code)
{
    int a = gc_get_int(machine->collector, machine->stack[machine->sp]);
    int b = gc_get_int(machine->collector, machine->stack[machine->sp - 1]);

    machine->stack[machine->sp - 1] = gc_alloc_int(machine->collector, a >= b);
    machine->sp--;
    
    /* printf("%d:%s\n", code->addr, __func__); */
}

void vm_execute_op_gte(vm * machine, bytecode * code)
{
    int a = gc_get_int(machine->collector, machine->stack[machine->sp]);
    int b = gc_get_int(machine->collector, machine->stack[machine->sp - 1]);

    machine->stack[machine->sp - 1] = gc_alloc_int(machine->collector, a <= b);
    machine->sp--;
    
    /* printf("%d:%s\n", code->addr, __func__); */
}

void vm_execute_op_eq(vm * machine, bytecode * code)
{
    int a = gc_get_int(machine->collector, machine->stack[machine->sp]);
    int b = gc_get_int(machine->collector, machine->stack[machine->sp - 1]);

    machine->stack[machine->sp - 1] = gc_alloc_int(machine->collector, a == b);
    machine->sp--;
    
    /* printf("%d:%s\n", code->addr, __func__); */
}

void vm_execute_func_def(vm * machine, bytecode * code)
{
    /* no op */
    /* printf("%d:%s\n", code->addr, __func__); */
}

void vm_execute_global_vec(vm * machine, bytecode * code)
{
    int c;
    unsigned int addr = gc_alloc_vec(machine->collector, code->global_vec.count);
    
    for (c = code->global_vec.count - 1; c >= 0; c--)
    {
        gc_set_vec(machine->collector, addr, c, machine->stack[machine->sp--]);
    }
    
    machine->sp++;
    vm_check_stack(machine);
    
    machine->stack[machine->sp] = addr;
    
    /* printf("%d:%s\n", code->addr, __func__); */
}

void vm_execute_mark(vm * machine, bytecode * code)
{
    machine->stack[machine->sp + 1] = machine->gp;
    machine->stack[machine->sp + 2] = machine->fp;
    machine->stack[machine->sp + 3] = code->mark.addr;
    
    machine->fp = machine->sp = machine->sp + 3;
    vm_check_stack(machine);

    /* printf("%d:%s\n", code->addr, __func__); */
}

void vm_execute_call(vm * machine, bytecode * code)
{
    unsigned int gp = gc_get_func_vec(machine->collector, machine->stack[machine->sp]);
    unsigned int ip = gc_get_func_addr(machine->collector, machine->stack[machine->sp]);

    machine->gp = gp;
    machine->ip = ip;
    machine->sp--;

    /* printf("%d:%s\n", code->addr, __func__); */
}

void vm_execute_ret(vm * machine, bytecode * code)
{
    machine->gp = machine->stack[machine->fp - 2];
    machine->ip = machine->stack[machine->fp];
    machine->stack[machine->fp - 2] = machine->stack[machine->sp];
    machine->sp = machine->fp - 2;
    machine->fp = machine->stack[machine->fp - 1];

    /* printf("%d:%s\n", code->addr, __func__); */
}

void vm_execute_line(vm * machine, bytecode * code)
{
    machine->line_no = code->line.no;
    
    /* printf("%d:%s\n", code->addr, __func__); */
}

void vm_execute_halt(vm * machine, bytecode * code)
{
    machine->running = 0;
    
    /* printf("%d:%s\n", code->addr, __func__); */
}

int vm_execute(vm * machine, bytecode * code, unsigned int size)
{
    bytecode * bc = NULL;
    
    machine->running = 1;
    while (machine->running)
    {
        bc = code + machine->ip;
        machine->ip++;
        vm_execute_op[bc->type].execute(machine, bc);
    }
    
    return gc_get_int(machine->collector, machine->stack[machine->sp]);
}

vm * vm_new(unsigned int stack_size, unsigned int mem_size)
{
    vm * machine = (vm *)malloc(sizeof(vm));

    machine->gp = -1;
    machine->fp = -1;
    machine->sp = -1;
    machine->ip = 0;
    machine->stack_size = stack_size;
    machine->stack = (unsigned int *)malloc(stack_size * sizeof(unsigned int));
    machine->collector = gc_new(mem_size);

    vm_execute_op_test();
        
    return machine;
}

void vm_delete(vm * machine)
{
    if (machine->stack != NULL)
    {
        free(machine->stack);
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
}

void vm_stack_print(vm * machine)
{
    int i = 0;
    
    for (i = 0; i <= machine->sp; i++)
    {
        printf("stack %d=%d\n", i, machine->stack[i]);
    }
}


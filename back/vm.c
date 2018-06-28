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
#include "gc.h"
#include "libmath.h"
#include "libvm.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

vm_execute_str vm_execute_op[] = {
    { BYTECODE_UNKNOWN, vm_execute_unknown },

    { BYTECODE_INT, vm_execute_int },
    { BYTECODE_FLOAT, vm_execute_float },

    { BYTECODE_ID_LOCAL, vm_execute_id_local },
    { BYTECODE_ID_GLOBAL, vm_execute_id_global },
    { BYTECODE_ID_FUNC_FUNC, vm_execute_id_func_func },
    { BYTECODE_ID_FUNC_ADDR, vm_execute_id_func_addr },

    { BYTECODE_OP_NEG_INT, vm_execute_op_neg_int },
    { BYTECODE_OP_ADD_INT, vm_execute_op_add_int },
    { BYTECODE_OP_SUB_INT, vm_execute_op_sub_int },
    { BYTECODE_OP_MUL_INT, vm_execute_op_mul_int },
    { BYTECODE_OP_DIV_INT, vm_execute_op_div_int },
    { BYTECODE_OP_MOD_INT, vm_execute_op_mod_int },

    { BYTECODE_OP_NEG_FLOAT, vm_execute_op_neg_float },
    { BYTECODE_OP_ADD_FLOAT, vm_execute_op_add_float },
    { BYTECODE_OP_SUB_FLOAT, vm_execute_op_sub_float },
    { BYTECODE_OP_MUL_FLOAT, vm_execute_op_mul_float },
    { BYTECODE_OP_DIV_FLOAT, vm_execute_op_div_float },

    { BYTECODE_OP_LT_INT, vm_execute_op_lt_int },
    { BYTECODE_OP_GT_INT, vm_execute_op_gt_int },
    { BYTECODE_OP_LTE_INT, vm_execute_op_lte_int },
    { BYTECODE_OP_GTE_INT, vm_execute_op_gte_int },
    { BYTECODE_OP_EQ_INT, vm_execute_op_eq_int },
    { BYTECODE_OP_NEQ_INT, vm_execute_op_neq_int },

    { BYTECODE_OP_LT_FLOAT, vm_execute_op_lt_float },
    { BYTECODE_OP_GT_FLOAT, vm_execute_op_gt_float },
    { BYTECODE_OP_LTE_FLOAT, vm_execute_op_lte_float },
    { BYTECODE_OP_GTE_FLOAT, vm_execute_op_gte_float },
    { BYTECODE_OP_EQ_FLOAT, vm_execute_op_eq_float },
    { BYTECODE_OP_NEQ_FLOAT, vm_execute_op_neq_float },

    { BYTECODE_OP_NOT_INT, vm_execute_op_not_int },

    { BYTECODE_INT_TO_FLOAT, vm_execute_int_to_float },
    { BYTECODE_FLOAT_TO_INT, vm_execute_float_to_int },

    { BYTECODE_JUMPZ, vm_execute_jumpz },
    { BYTECODE_JUMP, vm_execute_jump },
    { BYTECODE_LABEL, vm_execute_label },

    { BYTECODE_FUNC_DEF, vm_execute_func_def },
    { BYTECODE_GLOBAL_VEC, vm_execute_global_vec },
    { BYTECODE_MARK, vm_execute_mark },
    { BYTECODE_CALL, vm_execute_call },
    { BYTECODE_SLIDE, vm_execute_slide },
    { BYTECODE_RET, vm_execute_ret },
    { BYTECODE_LINE, vm_execute_line },
    { BYTECODE_BUILD_IN, vm_execute_build_in },
    { BYTECODE_COPYGLOB, vm_execute_copyglob },
    { BYTECODE_ALLOC, vm_execute_alloc },
    { BYTECODE_REWRITE, vm_execute_rewrite },
    { BYTECODE_PUSH_PARAM, vm_execute_push_param },

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

void vm_execute_int(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr addr = gc_alloc_int(machine->collector, code->integer.value);

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
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
    mem_ptr addr = machine
                       ->stack[machine->sp - (code->id_local.stack_level -
                                              code->id_local.index)]
                       .addr;

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}

void vm_execute_id_global(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr addr =
        gc_get_vec(machine->collector, machine->gp, code->id_global.index);

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}

void vm_execute_id_func_func(vm * machine, bytecode * code)
{
    fprintf(stderr, "at this stage id_func_func should be set to id_func_addr "
                    "with bytecode_func_addr\n");
    assert(0);
}

void vm_execute_id_func_addr(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    mem_ptr vec = machine->stack[machine->sp].addr;
    mem_ptr addr =
        gc_alloc_func(machine->collector, vec, code->id_func.func_addr);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}

/* a op b
 * b
 * a
 */
void vm_execute_op_neg_int(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    int a = gc_get_int(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, -a);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}

void vm_execute_op_add_int(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    int a =
        gc_get_int(machine->collector, machine->stack[machine->sp - 1].addr);
    int b = gc_get_int(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, a + b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_sub_int(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    int a =
        gc_get_int(machine->collector, machine->stack[machine->sp - 1].addr);
    int b = gc_get_int(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, a - b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_mul_int(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    int a =
        gc_get_int(machine->collector, machine->stack[machine->sp - 1].addr);
    int b = gc_get_int(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, a * b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_div_int(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    int a =
        gc_get_int(machine->collector, machine->stack[machine->sp - 1].addr);
    int b = gc_get_int(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = 0;

    if (b == 0)
    {
        print_error_msg(machine->line_no, "cannot divide by zero\n");
        machine->running = VM_ERROR;
        return;
    }

    addr = gc_alloc_int(machine->collector, a / b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_mod_int(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    int a = gc_get_int(machine->collector, machine->stack[machine->sp].addr);
    int b =
        gc_get_int(machine->collector, machine->stack[machine->sp - 1].addr);
    mem_ptr addr = 0;

    if (b == 0)
    {
        print_error_msg(machine->line_no, "cannot divide by zero\n");
        machine->running = VM_ERROR;
        return;
    }

    addr = gc_alloc_int(machine->collector, a % b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_neg_float(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a =
        gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_float(machine->collector, -a);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}

void vm_execute_op_add_float(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a =
        gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
    float b =
        gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_float(machine->collector, a + b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_sub_float(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a =
        gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
    float b =
        gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_float(machine->collector, a - b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_mul_float(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a =
        gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
    float b =
        gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_float(machine->collector, a * b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_div_float(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a =
        gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
    float b =
        gc_get_float(machine->collector, machine->stack[machine->sp].addr);
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

void vm_execute_op_lt_int(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    int a =
        gc_get_int(machine->collector, machine->stack[machine->sp - 1].addr);
    int b = gc_get_int(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, a < b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_gt_int(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    int a =
        gc_get_int(machine->collector, machine->stack[machine->sp - 1].addr);
    int b = gc_get_int(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, a > b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_lte_int(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    int a =
        gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
    int b = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, a >= b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_gte_int(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    int a =
        gc_get_int(machine->collector, machine->stack[machine->sp - 1].addr);
    int b = gc_get_int(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, a <= b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_eq_int(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    int a =
        gc_get_int(machine->collector, machine->stack[machine->sp - 1].addr);
    int b = gc_get_int(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, a == b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_neq_int(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    int a =
        gc_get_int(machine->collector, machine->stack[machine->sp - 1].addr);
    int b = gc_get_int(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, a != b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_lt_float(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a =
        gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
    float b =
        gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, a < b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_gt_float(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a =
        gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
    float b =
        gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, a > b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_lte_float(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a =
        gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
    float b =
        gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, a >= b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_gte_float(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a =
        gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
    float b =
        gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, a <= b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_eq_float(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a =
        gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
    float b =
        gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, a == b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_neq_float(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    float a =
        gc_get_float(machine->collector, machine->stack[machine->sp - 1].addr);
    float b =
        gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, a != b);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp - 1] = entry;
    machine->sp--;
}

void vm_execute_op_not_int(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    int a = gc_get_int(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, !a);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}

void vm_execute_int_to_float(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    int a = gc_get_int(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_float(machine->collector, (float)a);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}

void vm_execute_float_to_int(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };
    int a = gc_get_float(machine->collector, machine->stack[machine->sp].addr);
    mem_ptr addr = gc_alloc_int(machine->collector, (int)a);

    entry.type = GC_MEM_ADDR;
    entry.addr = addr;

    machine->stack[machine->sp] = entry;
}

void vm_execute_jumpz(vm * machine, bytecode * code)
{
    float a = gc_get_int(machine->collector, machine->stack[machine->sp].addr);

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

void vm_execute_label(vm * machine, bytecode * code) { /* no op */ }

void vm_execute_func_def(vm * machine, bytecode * code) { /* no op */ }

void vm_execute_global_vec(vm * machine, bytecode * code)
{
    int c;
    gc_stack entry = { 0 };
    mem_ptr addr = gc_alloc_vec(machine->collector, code->global_vec.count);

    for (c = code->global_vec.count - 1; c >= 0; c--)
    {
        gc_set_vec(machine->collector, addr, c,
                   machine->stack[machine->sp--].addr);
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
    mem_ptr gp =
        gc_get_func_vec(machine->collector, machine->stack[machine->sp].addr);
    ip_ptr ip =
        gc_get_func_addr(machine->collector, machine->stack[machine->sp].addr);

    machine->gp = gp;
    machine->ip = ip;
    machine->sp--;
}

void vm_execute_slide(vm * machine, bytecode * code)
{
    if (code->slide.q == 0)
    {
        return;
    }

    if (code->slide.m == 0)
    {
        machine->sp = machine->sp - code->slide.q;
    }
    else
    {
        unsigned int i;
        machine->sp = machine->sp - code->slide.q - code->slide.m;

        for (i = 0; i < code->slide.m; i++)
        {
            machine->sp++;
            machine->stack[machine->sp] =
                machine->stack[machine->sp + code->slide.q];
        }
    }
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
    libvm_execute_build_in(machine, code);
}

void vm_execute_copyglob(vm * machine, bytecode * code)
{
    gc_stack entry = { 0 };

    machine->sp++;
    vm_check_stack(machine);

    entry.type = GC_MEM_ADDR;
    entry.addr = machine->gp;

    machine->stack[machine->sp] = entry;
}

void vm_execute_alloc(vm * machine, bytecode * code)
{
    unsigned int i;

    for (i = 0; i < code->alloc.n; i++)
    {
        gc_stack entry = { 0 };
        mem_ptr addr = gc_alloc_func(machine->collector, 0, 0);

        entry.type = GC_MEM_ADDR;
        entry.addr = addr;

        machine->sp++;
        machine->stack[machine->sp] = entry;
    }

    vm_check_stack(machine);
}

void vm_execute_rewrite(vm * machine, bytecode * code)
{
    unsigned int j = code->rewrite.j;

    mem_ptr gp =
        gc_get_func_vec(machine->collector, machine->stack[machine->sp].addr);
    ip_ptr ip =
        gc_get_func_addr(machine->collector, machine->stack[machine->sp].addr);

    gc_set_func_vec(machine->collector, machine->stack[machine->sp - j].addr,
                    gp);
    gc_set_func_addr(machine->collector, machine->stack[machine->sp - j].addr,
                     ip);

    machine->sp--;
    vm_check_stack(machine);
}

void vm_execute_push_param(vm * machine, bytecode * code)
{
    mem_ptr addr;
    gc_stack entry = { 0 };
    unsigned int i;

    for (i = machine->prog->param_count; i > 0; i--)
    {
        if (machine->prog->params[i - 1].type == OBJECT_INT)
        {
            addr = gc_alloc_int(machine->collector,
                                machine->prog->params[i - 1].int_value);
        }
        else if (machine->prog->params[i - 1].type == OBJECT_FLOAT)
        {
            addr = gc_alloc_float(machine->collector,
                                  machine->prog->params[i - 1].float_value);
        }
        else
        {
            printf("unsupported type\n");
            assert(0);
        }

        machine->sp++;
        vm_check_stack(machine);

        entry.type = GC_MEM_ADDR;
        entry.addr = addr;

        machine->stack[machine->sp] = entry;
    }
}

void vm_execute_halt(vm * machine, bytecode * code)
{
    machine->running = VM_HALT;
}

int vm_execute(vm * machine, program * prog, object * result)
{
    bytecode * bc = NULL;

    machine->prog = prog;
    machine->running = VM_RUNNING;
    while (machine->running == VM_RUNNING)
    {
        bc = prog->code_arr + machine->ip;
        machine->ip++;
        vm_execute_op[bc->type].execute(machine, bc);
    }

    if (machine->running == VM_HALT)
    {
        *result =
            gc_get_object(machine->collector, machine->stack[machine->sp].addr);
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

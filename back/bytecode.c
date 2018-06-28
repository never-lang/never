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
#include "bytecode.h"
#include "func.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

bytecode_op_str bytecode_op[] = {
    { BYTECODE_UNKNOWN, bytecode_print_unknown },

    { BYTECODE_INT, bytecode_print_int },
    { BYTECODE_FLOAT, bytecode_print_float },

    { BYTECODE_ID_LOCAL, bytecode_print_id_local },
    { BYTECODE_ID_GLOBAL, bytecode_print_id_global },
    { BYTECODE_ID_FUNC_FUNC, bytecode_print_id_func_func },
    { BYTECODE_ID_FUNC_ADDR, bytecode_print_id_func_addr },

    { BYTECODE_OP_NEG_INT, bytecode_print_op_neg_int },
    { BYTECODE_OP_ADD_INT, bytecode_print_op_add_int },
    { BYTECODE_OP_SUB_INT, bytecode_print_op_sub_int },
    { BYTECODE_OP_MUL_INT, bytecode_print_op_mul_int },
    { BYTECODE_OP_DIV_INT, bytecode_print_op_div_int },
    { BYTECODE_OP_MOD_INT, bytecode_print_op_mod_int },

    { BYTECODE_OP_NEG_FLOAT, bytecode_print_op_neg_float },
    { BYTECODE_OP_ADD_FLOAT, bytecode_print_op_add_float },
    { BYTECODE_OP_SUB_FLOAT, bytecode_print_op_sub_float },
    { BYTECODE_OP_MUL_FLOAT, bytecode_print_op_mul_float },
    { BYTECODE_OP_DIV_FLOAT, bytecode_print_op_div_float },

    { BYTECODE_OP_LT_INT, bytecode_print_op_lt_int },
    { BYTECODE_OP_GT_INT, bytecode_print_op_gt_int },
    { BYTECODE_OP_LTE_INT, bytecode_print_op_lte_int },
    { BYTECODE_OP_GTE_INT, bytecode_print_op_gte_int },
    { BYTECODE_OP_EQ_INT, bytecode_print_op_eq_int },
    { BYTECODE_OP_NEQ_INT, bytecode_print_op_neq_int },

    { BYTECODE_OP_LT_FLOAT, bytecode_print_op_lt_float },
    { BYTECODE_OP_GT_FLOAT, bytecode_print_op_gt_float },
    { BYTECODE_OP_LTE_FLOAT, bytecode_print_op_lte_float },
    { BYTECODE_OP_GTE_FLOAT, bytecode_print_op_gte_float },
    { BYTECODE_OP_EQ_FLOAT, bytecode_print_op_eq_float },
    { BYTECODE_OP_NEQ_FLOAT, bytecode_print_op_neq_float },

    { BYTECODE_OP_NOT_INT, bytecode_print_op_not_int },

    { BYTECODE_INT_TO_FLOAT, bytecode_print_int_to_float },
    { BYTECODE_FLOAT_TO_INT, bytecode_print_float_to_int },

    { BYTECODE_JUMPZ, bytecode_print_jumpz },
    { BYTECODE_JUMP, bytecode_print_jump },
    { BYTECODE_LABEL, bytecode_print_label },

    { BYTECODE_FUNC_DEF, bytecode_print_func_def },
    { BYTECODE_GLOBAL_VEC, bytecode_print_global_vec },
    { BYTECODE_MARK, bytecode_print_mark },
    { BYTECODE_CALL, bytecode_print_call },
    { BYTECODE_SLIDE, bytecode_print_slide },
    { BYTECODE_RET, bytecode_print_ret },
    { BYTECODE_LINE, bytecode_print_line },
    { BYTECODE_BUILD_IN, bytecode_print_build_in },
    { BYTECODE_COPYGLOB, bytecode_print_copyglob },
    { BYTECODE_ALLOC, bytecode_print_alloc },
    { BYTECODE_REWRITE, bytecode_print_rewrite },
    { BYTECODE_PUSH_PARAM, bytecode_print_push_param },

    { BYTECODE_HALT, bytecode_print_halt }
};

static void bytecode_op_test()
{
    unsigned int i = 0;
    for (i = 0; i < BYTECODE_END; i++)
    {
        assert(i == bytecode_op[i].type);
    }
}

void bytecode_print_unknown(bytecode * code)
{
    printf("%d: unknown\n", code->addr);
}

void bytecode_print_int(bytecode * code)
{
    printf("%d: int %d\n", code->addr, code->integer.value);
}

void bytecode_print_float(bytecode * code)
{
    printf("%d: float %f\n", code->addr, code->real.value);
}

void bytecode_print_id_local(bytecode * code)
{
    printf("%d: id local %d %d\n", code->addr, code->id_local.stack_level,
           code->id_local.index);
}

void bytecode_print_id_global(bytecode * code)
{
    printf("%d: id global %d\n", code->addr, code->id_global.index);
}

void bytecode_print_id_func_func(bytecode * code)
{
    printf("%d: id func func %d\n", code->addr, code->id_func.func_value->addr);
}

void bytecode_print_id_func_addr(bytecode * code)
{
    printf("%d: id func addr %d\n", code->addr, code->id_func.func_addr);
}

void bytecode_print_op_neg_int(bytecode * code)
{
    printf("%d: op neg int\n", code->addr);
}

void bytecode_print_op_add_int(bytecode * code)
{
    printf("%d: op add int\n", code->addr);
}

void bytecode_print_op_sub_int(bytecode * code)
{
    printf("%d: op sub int\n", code->addr);
}

void bytecode_print_op_mul_int(bytecode * code)
{
    printf("%d: op mul int\n", code->addr);
}

void bytecode_print_op_div_int(bytecode * code)
{
    printf("%d: op div int\n", code->addr);
}

void bytecode_print_op_mod_int(bytecode * code)
{
    printf("%d: op mod int\n", code->addr);
}

void bytecode_print_op_neg_float(bytecode * code)
{
    printf("%d: op neg float\n", code->addr);
}

void bytecode_print_op_add_float(bytecode * code)
{
    printf("%d: op add float\n", code->addr);
}

void bytecode_print_op_sub_float(bytecode * code)
{
    printf("%d: op sub float\n", code->addr);
}

void bytecode_print_op_mul_float(bytecode * code)
{
    printf("%d: op mul float\n", code->addr);
}

void bytecode_print_op_div_float(bytecode * code)
{
    printf("%d: op div float\n", code->addr);
}

void bytecode_print_op_lt_int(bytecode * code)
{
    printf("%d: op lt int\n", code->addr);
}

void bytecode_print_op_gt_int(bytecode * code)
{
    printf("%d: op gt int\n", code->addr);
}

void bytecode_print_op_lte_int(bytecode * code)
{
    printf("%d: op lte int\n", code->addr);
}

void bytecode_print_op_gte_int(bytecode * code)
{
    printf("%d: op gte int\n", code->addr);
}

void bytecode_print_op_eq_int(bytecode * code)
{
    printf("%d: op eq int\n", code->addr);
}

void bytecode_print_op_neq_int(bytecode * code)
{
    printf("%d: op neq int\n", code->addr);
}

void bytecode_print_op_lt_float(bytecode * code)
{
    printf("%d: op lt float\n", code->addr);
}

void bytecode_print_op_gt_float(bytecode * code)
{
    printf("%d: op gt float\n", code->addr);
}

void bytecode_print_op_lte_float(bytecode * code)
{
    printf("%d: op lte float\n", code->addr);
}

void bytecode_print_op_gte_float(bytecode * code)
{
    printf("%d: op gte float\n", code->addr);
}

void bytecode_print_op_eq_float(bytecode * code)
{
    printf("%d: op eq float\n", code->addr);
}

void bytecode_print_op_neq_float(bytecode * code)
{
    printf("%d: op neq_float\n", code->addr);
}

void bytecode_print_op_not_int(bytecode * code)
{
    printf("%d: op not\n", code->addr);
}

void bytecode_print_int_to_float(bytecode * code)
{
    printf("%d: int to float\n", code->addr);
}

void bytecode_print_float_to_int(bytecode * code)
{
    printf("%d: float to int\n", code->addr);
}

void bytecode_print_jumpz(bytecode * code)
{
    printf("%d: jumpz %d (%d)\n", code->addr, code->jump.offset,
           code->addr + code->jump.offset);
}

void bytecode_print_jump(bytecode * code)
{
    printf("%d: jump %d (%d)\n", code->addr, code->jump.offset,
           code->addr + code->jump.offset);
}

void bytecode_print_label(bytecode * code)
{
    printf("%d: label\n", code->addr);
}

void bytecode_print_func_def(bytecode * code)
{
    printf("\n%d: func def\n", code->addr);
}

void bytecode_print_global_vec(bytecode * code)
{
    printf("%d: global vec %d\n", code->addr, code->global_vec.count);
}

void bytecode_print_mark(bytecode * code)
{
    printf("%d: mark %u\n", code->addr, code->mark.addr);
}

void bytecode_print_call(bytecode * code) { printf("%d: call\n", code->addr); }

void bytecode_print_slide(bytecode * code)
{
    printf("%d: last call %u %u\n", code->addr, code->slide.q, code->slide.m);
}

void bytecode_print_ret(bytecode * code)
{
    printf("%d: ret %d\n", code->addr, code->ret.count);
}

void bytecode_print_line(bytecode * code)
{
    printf("%d: line %u\n", code->addr, code->line.no);
}

void bytecode_print_build_in(bytecode * code)
{
    printf("%d: build in id %u\n", code->addr, code->build_in.id);
}

void bytecode_print_copyglob(bytecode * code)
{
    printf("%d: copyglob\n", code->addr);
}

void bytecode_print_alloc(bytecode * code)
{
    printf("%d: alloc %u\n", code->addr, code->alloc.n);
}

void bytecode_print_rewrite(bytecode * code)
{
    printf("%d: rewrite %u\n", code->addr, code->rewrite.j);
}

void bytecode_print_push_param(bytecode * code)
{
    printf("%d: push param\n", code->addr);
}

void bytecode_print_halt(bytecode * code) { printf("%d: halt\n", code->addr); }

bytecode_list_node * bytecode_list_node_new(bytecode * value)
{
    bytecode_list_node * node =
        (bytecode_list_node *)malloc(sizeof(bytecode_list_node));

    node->value = *value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void bytecode_list_node_delete(bytecode_list_node * node) { free(node); }

bytecode_list * bytecode_new()
{
    bytecode_list * list = (bytecode_list *)malloc(sizeof(bytecode_list));

    list->addr = 0;
    list->head = NULL;
    list->tail = NULL;

    bytecode_op_test();

    return list;
}

void bytecode_delete(bytecode_list * code)
{
    bytecode_list_node * node = code->tail;
    while (node != NULL)
    {
        bytecode_list_node * tmp = node->next;
        bytecode_list_node_delete(node);
        node = tmp;
    }
    free(code);
}

bytecode * bytecode_add(bytecode_list * code, bytecode * value)
{
    bytecode_list_node * node;

    value->addr = code->addr++;
    node = bytecode_list_node_new(value);

    if (code->head == NULL && code->tail == NULL)
    {
        code->head = code->tail = node;
    }
    else
    {
        code->head->next = node;
        node->prev = code->head;
        code->head = node;
    }

    return &node->value;
}

void bytecode_func_addr(bytecode_list * code)
{
    bytecode_list_node * node = code->tail;
    while (node != NULL)
    {
        bytecode * code = &node->value;
        if (code != NULL)
        {
            if (code->type == BYTECODE_ID_FUNC_FUNC)
            {
                code->type = BYTECODE_ID_FUNC_ADDR;
                code->id_func.func_addr = code->id_func.func_value->addr;
            }
        }
        node = node->next;
    }
}

void bytecode_print(bytecode_list * code)
{
    bytecode_list_node * node = code->tail;

    printf("---- bytecode beg ---\n");

    while (node != NULL)
    {
        bytecode * code = &node->value;
        if (code != NULL)
        {
            bytecode_op[code->type].print(code);
        }
        node = node->next;
    }

    printf("---- bytecode end ---\n");
}

void bytecode_to_array(bytecode_list * code, bytecode ** code_arr,
                       unsigned int * code_size)
{
    unsigned int addr = 0;
    bytecode_list_node * node = NULL;

    *code_size = code->addr;
    *code_arr = (bytecode *)malloc(code->addr * sizeof(bytecode));

    node = code->tail;
    while (node != NULL)
    {
        if (node->value.addr != addr)
        {
            fprintf(stderr, "incorrectly generated code\n");
            assert(0);
        }
        if (node->value.type == BYTECODE_ID_FUNC_FUNC)
        {
            fprintf(stderr, "cannot generate bytecode array with function "
                            "pointers, use bytecode_func_addr\n");
            assert(0);
        }

        (*code_arr)[addr++] = node->value;

        node = node->next;
    }
}

void bytecode_array_delete(bytecode * code_arr) { free(code_arr); }

void bytecode_array_print(bytecode * code_arr, unsigned int size)
{
    unsigned int i;

    printf("---- bytecode array beg ---\n");

    for (i = 0; i < size; i++)
    {
        bytecode_op[code_arr[i].type].print(code_arr + i);
    }

    printf("---- bytecode array end ---\n");
}

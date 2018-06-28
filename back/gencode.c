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
#include "gencode.h"
#include "freevar.h"
#include "symtab.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>

/* GP old, FP old, IP old */
#define NUM_FRAME_PTRS 3

int func_enum_vars(func * func_value)
{
    int index = 0;
    var_list_node * node = NULL;

    node = func_value->vars->tail;
    while (node != NULL)
    {
        var * value = node->value;
        if (value != NULL)
        {
            value->index = -(index++);
        }
        node = node->next;
    }
    return 0;
}

int func_enum_funcs(func * func_value)
{
    int index = 1;
    func_list * list = NULL;

    list = func_value->body->funcs;
    if (list != NULL)
    {
        func_list_node * node = list->tail;
        while (node != NULL)
        {
            func * value = node->value;
            if (value != NULL)
            {
                value->index = index++;
            }
            node = node->next;
        }
    }

    return 0;
}

/**
 * free variables
 */
int expr_id_gencode(unsigned int syn_level, func * func_value, expr * value,
                    int * result)
{
    symtab_entry * entry = NULL;

    entry = symtab_lookup(func_value->stab, value->id.id, SYMTAB_NESTED);
    if (entry != NULL)
    {
        if (entry->type == SYMTAB_FUNC && entry->func_value != NULL)
        {
            func * sup_func_value = entry->func_value;
            if (sup_func_value)
            {
                if (entry->syn_level == 0)
                {
                    value->id.id_type_value = ID_TYPE_FUNC_TOP;
                    value->id.id_func_value = sup_func_value;
                }
                else if (syn_level == entry->syn_level &&
                         func_value == sup_func_value) /* recursive call */
                {
                    value->id.id_type_value = ID_TYPE_FUNC_NEST;
                    value->id.id_func_value = sup_func_value;
                }
                else if (syn_level == entry->syn_level)
                {
                    value->id.id_type_value = ID_TYPE_FUNC;
                    value->id.id_func_value = sup_func_value;
                }
                else
                {
                    freevar * freevar_value = NULL;
                    if (func_value->freevars == NULL)
                    {
                        func_value->freevars = freevar_list_new();
                    }

                    freevar_value =
                        freevar_list_add(func_value->freevars, value->id.id);

                    freevar_value->type = FREEVAR_FUNC;
                    freevar_value->func_value = sup_func_value;

                    value->id.id_type_value = ID_TYPE_GLOBAL;
                    value->id.id_freevar_value = freevar_value;
                }
            }
        }
        else if (entry->type == SYMTAB_VAR && entry->var_value != NULL)
        {
            var * var_value = entry->var_value;
            if (var_value->type == VAR_INT || var_value->type == VAR_FLOAT ||
                var_value->type == VAR_FUNC)
            {
                if (syn_level == entry->syn_level)
                {
                    value->id.id_type_value = ID_TYPE_LOCAL;
                    value->id.id_var_value = var_value;
                }
                else
                {
                    freevar * freevar_value = NULL;
                    if (func_value->freevars == NULL)
                    {
                        func_value->freevars = freevar_list_new();
                    }

                    freevar_value =
                        freevar_list_add(func_value->freevars, value->id.id);

                    value->id.id_type_value = ID_TYPE_GLOBAL;
                    value->id.id_freevar_value = freevar_value;
                }
            }
        }
    }
    else
    {
        *result = 1;
        print_error_msg(
            value->line_no,
            "cannot find variable %s, at this stage it is very bad\n",
            value->id);
        assert(0);
    }

    return 0;
}

int expr_gencode(unsigned int syn_level, func * func_value, expr * value,
                 int * result)
{
    switch (value->type)
    {
    case EXPR_INT:
        break;
    case EXPR_FLOAT:
        /* printf("gencode EXPR_FLOAT %f\n", value->float_value); */
        break;
    case EXPR_ID:
        expr_id_gencode(syn_level, func_value, value, result);
        break;
    case EXPR_NEG:
        expr_gencode(syn_level, func_value, value->left, result);
        break;
    case EXPR_ADD:
    case EXPR_SUB:
    case EXPR_MUL:
    case EXPR_DIV:
    case EXPR_MOD:
    case EXPR_LT:
    case EXPR_GT:
    case EXPR_LTE:
    case EXPR_GTE:
    case EXPR_EQ:
    case EXPR_NEQ:
        expr_gencode(syn_level, func_value, value->left, result);
        expr_gencode(syn_level, func_value, value->right, result);
        break;
    case EXPR_AND:
    case EXPR_OR:
        expr_gencode(syn_level, func_value, value->left, result);
        expr_gencode(syn_level, func_value, value->right, result);
        break;
    case EXPR_NOT:
        expr_gencode(syn_level, func_value, value->left, result);
        break;
    case EXPR_SUP:
        expr_gencode(syn_level, func_value, value->left, result);
        break;
    case EXPR_COND:
        expr_gencode(syn_level, func_value, value->left, result);
        expr_gencode(syn_level, func_value, value->middle, result);
        expr_gencode(syn_level, func_value, value->right, result);
        break;
    case EXPR_CALL:
    case EXPR_LAST_CALL:
        expr_gencode(syn_level, func_value, value->call.func_expr, result);
        if (value->call.vars)
        {
            expr_list_gencode(syn_level, func_value, value->call.vars, result);
        }
        break;
    case EXPR_FUNC:
        if (value->func_value)
        {
            func_gencode(syn_level + 1, value->func_value, result);
        }
        break;
    case EXPR_BUILD_IN:
        expr_list_gencode(syn_level, func_value, value->func_build_in.param,
                          result);
        break;
    case EXPR_INT_TO_FLOAT:
    case EXPR_FLOAT_TO_INT:
        expr_gencode(syn_level, func_value, value->left, result);
        break;
    }
    return 0;
}

int expr_list_gencode(unsigned int syn_level, func * func_value,
                      expr_list * list, int * result)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value)
        {
            expr_gencode(syn_level, func_value, value, result);
        }
        node = node->next;
    }
    return 0;
}

/**
 * free variables
 */
int func_gencode_freevars_freevar(func * func_value, freevar * freevar_value,
                                  int * result)
{
    /** search in symtab */
    /** if found then mark as local */
    /** otherwise mark as global and set index */
    symtab_entry * entry = NULL;

    entry = symtab_lookup(func_value->stab, freevar_value->id, SYMTAB_FLAT);
    if (entry != NULL)
    {
        if (entry->type == SYMTAB_FUNC && entry->func_value)
        {
            freevar_value->type = FREEVAR_FUNC;
            freevar_value->func_value = entry->func_value;
        }
        else if (entry->type == SYMTAB_VAR && entry->var_value)
        {
            freevar_value->type = FREEVAR_LOCAL;
            freevar_value->local_value = entry->var_value;
        }
    }
    else
    {
        freevar * freevar_sup_value = NULL;
        if (func_value->freevars == NULL)
        {
            func_value->freevars = freevar_list_new();
        }

        freevar_sup_value =
            freevar_list_add(func_value->freevars, freevar_value->id);

        freevar_value->type = FREEVAR_GLOBAL;
        freevar_value->global_value = freevar_sup_value;
    }

    return 0;
}

int func_gencode_freevars_expr(func * func_value, expr * value, int * result)
{
    switch (value->type)
    {
    case EXPR_INT:
        /* not possible */
        break;
    case EXPR_FLOAT:
        /* not possible */
        break;
    case EXPR_ID:
        /* not possible */
        break;
    case EXPR_NEG:
        func_gencode_freevars_expr(func_value, value->left, result);
        break;
    case EXPR_ADD:
    case EXPR_SUB:
    case EXPR_MUL:
    case EXPR_DIV:
    case EXPR_MOD:
    case EXPR_LT:
    case EXPR_GT:
    case EXPR_LTE:
    case EXPR_GTE:
    case EXPR_EQ:
    case EXPR_NEQ:
        func_gencode_freevars_expr(func_value, value->left, result);
        func_gencode_freevars_expr(func_value, value->right, result);
        break;
    case EXPR_AND:
    case EXPR_OR:
        func_gencode_freevars_expr(func_value, value->left, result);
        func_gencode_freevars_expr(func_value, value->right, result);
        break;
    case EXPR_NOT:
        func_gencode_freevars_expr(func_value, value->left, result);
        break;
    case EXPR_SUP:
        func_gencode_freevars_expr(func_value, value->left, result);
        break;
    case EXPR_COND:
        func_gencode_freevars_expr(func_value, value->left, result);
        func_gencode_freevars_expr(func_value, value->middle, result);
        func_gencode_freevars_expr(func_value, value->right, result);
        break;
    case EXPR_CALL:
    case EXPR_LAST_CALL:
        func_gencode_freevars_expr(func_value, value->call.func_expr, result);
        if (value->call.vars)
        {
            func_gencode_freevars_expr_list(func_value, value->call.vars,
                                            result);
        }
        break;
    case EXPR_FUNC:
        if (value->func_value)
        {
            func_gencode_freevars_func(func_value, value->func_value, result);
        }
        break;
    case EXPR_BUILD_IN:
        func_gencode_freevars_expr_list(func_value, value->func_build_in.param,
                                        result);
        break;
    case EXPR_INT_TO_FLOAT:
    case EXPR_FLOAT_TO_INT:
        func_gencode_freevars_expr(func_value, value->left, result);
        break;
    }

    return 0;
}

int func_gencode_freevars_expr_list(func * func_value, expr_list * list,
                                    int * result)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            func_gencode_freevars_expr(func_value, value, result);
        }
        node = node->next;
    }

    return 0;
}

int func_gencode_freevars_func(func * func_value, func * subfunc_value,
                               int * result)
{
    if (subfunc_value->freevars)
    {
        freevar_list_node * node = subfunc_value->freevars->tail;
        while (node != NULL)
        {
            freevar * freevar_value = node->value;
            if (freevar_value != NULL)
            {
                func_gencode_freevars_freevar(func_value, freevar_value,
                                              result);
            }
            node = node->next;
        }
    }

    return 0;
}

int func_gencode_freevars_func_list(func * func_value, func_list * list,
                                    int * result)
{
    func_list_node * node = list->tail;

    while (node != NULL)
    {
        func * subfunc_value = node->value;
        if (subfunc_value != NULL)
        {
            func_gencode_freevars_func(func_value, subfunc_value, result);
        }
        node = node->next;
    }

    return 0;
}

int func_gencode_freevars(func * func_value, int * result)
{
    if (func_value->body && func_value->body->funcs)
    {
        func_gencode_freevars_func_list(func_value, func_value->body->funcs,
                                        result);
    }
    if (func_value->body && func_value->body->ret)
    {
        func_gencode_freevars_expr(func_value, func_value->body->ret, result);
    }

    return 0;
}

int func_gencode(unsigned int syn_level, func * func_value, int * result)
{
    if (func_value->stab != NULL)
    {
        symtab_set_syn_level(func_value->stab, syn_level);
    }
    if (func_value->vars != NULL)
    {
        func_enum_vars(func_value);
    }
    if (func_value->body != NULL)
    {
        func_enum_funcs(func_value);
    }

    if (func_value->body && func_value->body->funcs)
    {
        func_list_gencode(syn_level, func_value->body->funcs, result);
    }
    if (func_value->body && func_value->body->ret)
    {
        expr_gencode(syn_level, func_value, func_value->body->ret, result);
    }

    /** set subfunction local/global indexes **/
    func_gencode_freevars(func_value, result);

    return 0;
}

int func_list_gencode(unsigned int syn_level, func_list * list, int * result)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value)
        {
            func_gencode(syn_level + 1, func_value, result);
        }
        node = node->next;
    }
    return 0;
}

int never_gencode(never * nev)
{
    int gencode_res = GENCODE_SUCC;
    unsigned int syn_level = 0;

    if (nev->funcs)
    {
        if (nev->stab != NULL)
        {
            symtab_set_syn_level(nev->stab, syn_level);
        }
        func_list_gencode(syn_level, nev->funcs, &gencode_res);
    }

    return gencode_res;
}

/**
 * emit code
 */
int expr_int_emit(expr * value, int stack_level, bytecode_list * code,
                  int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_INT;
    bc.integer.value = value->int_value;

    bytecode_add(code, &bc);

    return 0;
}

int expr_float_emit(expr * value, int stack_level, bytecode_list * code,
                    int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_FLOAT;
    bc.real.value = value->float_value;

    bytecode_add(code, &bc);

    return 0;
}

int func_freevar_emit(freevar * value, int stack_level, bytecode_list * code,
                      int * result)
{
    bytecode bc = { 0 };

    switch (value->type)
    {
    case FREEVAR_UNKNOWN:
        print_error_msg(0, "unknown freevar %s during emit\n", value->id);
        assert(0);
        break;
    case FREEVAR_LOCAL:
        bc.type = BYTECODE_ID_LOCAL;
        bc.id_local.stack_level = stack_level;
        bc.id_local.index = value->local_value->index;

        bytecode_add(code, &bc);
        break;
    case FREEVAR_GLOBAL:
        bc.type = BYTECODE_ID_GLOBAL;
        bc.id_global.index = value->global_value->index;

        bytecode_add(code, &bc);
        break;
    case FREEVAR_FUNC:
        bc.type = BYTECODE_ID_LOCAL;
        bc.id_local.stack_level = stack_level;
        bc.id_local.index = value->func_value->index;

        bytecode_add(code, &bc);
        break;
    }

    return 0;
}

int func_freevar_list_emit(freevar_list * freevars, int stack_level,
                           bytecode_list * code, int * result)
{
    int e = 0;
    freevar_list_node * node = freevars->tail;
    while (node != NULL)
    {
        freevar * value = node->value;
        if (value != NULL)
        {
            func_freevar_emit(value, stack_level + e++, code, result);
        }
        node = node->next;
    }

    return 0;
}

int expr_id_func_top_emit(func * func_value, int stack_level,
                          bytecode_list * code, int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_GLOBAL_VEC;
    bc.global_vec.count = 0;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_ID_FUNC_FUNC;
    bc.id_func.func_value = func_value;
    bytecode_add(code, &bc);

    return 0;
}

int expr_id_func_emit(func * func_value, int stack_level, bytecode_list * code,
                      int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_ID_LOCAL;
    bc.id_local.stack_level = stack_level;
    bc.id_local.index = func_value->index;

    bytecode_add(code, &bc);

    return 0;
}

int expr_id_func_nest_emit(func * func_value, int stack_level,
                           bytecode_list * code, int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_COPYGLOB;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_ID_FUNC_FUNC;
    bc.id_func.func_value = func_value;
    bytecode_add(code, &bc);

    return 0;
}

int expr_id_emit(expr * value, int stack_level, bytecode_list * code,
                 int * result)
{
    switch (value->id.id_type_value)
    {
    case ID_TYPE_UNKNOWN:
        print_error_msg(value->line_no,
                        "not recognized id, at this stage it is very bad\n");
        assert(0);
        break;
    case ID_TYPE_LOCAL:
    {
        bytecode bc = { 0 };

        bc.type = BYTECODE_ID_LOCAL;
        bc.id_local.stack_level = stack_level;
        bc.id_local.index = value->id.id_var_value->index;

        bytecode_add(code, &bc);
    }
    break;
    case ID_TYPE_GLOBAL:
    {
        bytecode bc = { 0 };

        bc.type = BYTECODE_ID_GLOBAL;
        bc.id_global.index = value->id.id_freevar_value->index;

        bytecode_add(code, &bc);
    }
    break;
    case ID_TYPE_FUNC_TOP:
        if (value->id.id_func_value != NULL)
        {
            expr_id_func_top_emit(value->id.id_func_value, stack_level, code,
                                  result);
        }
        break;
    case ID_TYPE_FUNC:
        if (value->id.id_func_value != NULL)
        {
            expr_id_func_emit(value->id.id_func_value, stack_level, code,
                              result);
        }
        break;
    case ID_TYPE_FUNC_NEST:
        if (value->id.id_func_value != NULL)
        {
            expr_id_func_nest_emit(value->id.id_func_value, stack_level, code,
                                   result);
        }
        break;
    }
    return 0;
}

int expr_and_emit(expr * value, int stack_level, bytecode_list * code,
                  int * result)
{
    bytecode bc = { 0 };
    bytecode *condzA, *condzB, *jumpE;
    bytecode *labelEF, *labelE;

    expr_emit(value->left, stack_level, code, result);

    bc.type = BYTECODE_JUMPZ;
    condzA = bytecode_add(code, &bc);

    expr_emit(value->right, stack_level, code, result);

    bc.type = BYTECODE_JUMPZ;
    condzB = bytecode_add(code, &bc);

    bc.type = BYTECODE_INT;
    bc.integer.value = 1;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_JUMP;
    jumpE = bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    labelEF = bytecode_add(code, &bc);
    condzA->jump.offset = labelEF->addr - condzA->addr;
    condzB->jump.offset = labelEF->addr - condzB->addr;

    bc.type = BYTECODE_INT;
    bc.integer.value = 0;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    labelE = bytecode_add(code, &bc);
    jumpE->jump.offset = labelE->addr - jumpE->addr;

    return 0;
}

int expr_or_emit(expr * value, int stack_level, bytecode_list * code,
                 int * result)
{
    bytecode bc = { 0 };
    bytecode *condzA, *condzB, *jumpET, *jumpE;
    bytecode *labelB, *labelET, *labelEF, *labelE;

    expr_emit(value->left, stack_level, code, result);

    bc.type = BYTECODE_JUMPZ;
    condzA = bytecode_add(code, &bc);

    bc.type = BYTECODE_JUMP;
    jumpET = bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    labelB = bytecode_add(code, &bc);
    condzA->jump.offset = labelB->addr - condzA->addr;

    expr_emit(value->right, stack_level, code, result);

    bc.type = BYTECODE_JUMPZ;
    condzB = bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    labelET = bytecode_add(code, &bc);
    jumpET->jump.offset = labelET->addr - jumpET->addr;

    bc.type = BYTECODE_INT;
    bc.integer.value = 1;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_JUMP;
    jumpE = bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    labelEF = bytecode_add(code, &bc);
    condzB->jump.offset = labelEF->addr - condzB->addr;

    bc.type = BYTECODE_INT;
    bc.integer.value = 0;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    labelE = bytecode_add(code, &bc);
    jumpE->jump.offset = labelE->addr - jumpE->addr;

    return 0;
}

int expr_not_emit(expr * value, int stack_level, bytecode_list * code,
                  int * result)
{
    bytecode bc = { 0 };
    expr_emit(value->left, stack_level, code, result);

    if (value->comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_NOT_INT;
        bytecode_add(code, &bc);
    }
    else
    {
        *result = GENCODE_FAIL;
        print_error_msg(value->line_no, "cannot not type %s\n",
                        comb_type_str(value->comb));
    }

    return 0;
}

int expr_cond_emit(expr * value, int stack_level, bytecode_list * code,
                   int * result)
{
    bytecode bc = { 0 };
    bytecode *cond, *condz;
    bytecode *labelA, *labelB;

    expr_emit(value->left, stack_level, code, result);

    bc.type = BYTECODE_JUMPZ;
    condz = bytecode_add(code, &bc);

    expr_emit(value->middle, stack_level, code, result);

    bc.type = BYTECODE_JUMP;
    cond = bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    labelA = bytecode_add(code, &bc);
    condz->jump.offset = labelA->addr - condz->addr;

    expr_emit(value->right, stack_level, code, result);

    bc.type = BYTECODE_LABEL;
    labelB = bytecode_add(code, &bc);
    cond->jump.offset = labelB->addr - cond->addr;

    return 0;
}

int expr_call_emit(expr * value, int stack_level, bytecode_list * code,
                   int * result)
{
    int v = NUM_FRAME_PTRS;
    bytecode bc = { 0 };
    bytecode *mark, *label;

    bc.type = BYTECODE_MARK;
    mark = bytecode_add(code, &bc);

    if (value->call.vars)
    {
        expr_list_emit(value->call.vars, stack_level + v, code, result);
        v += value->call.vars->count;
    }
    expr_emit(value->call.func_expr, stack_level + v, code, result);

    bc.type = BYTECODE_CALL;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    label = bytecode_add(code, &bc);
    mark->mark.addr = label->addr;

    return 0;
}

int expr_last_call_emit(expr * value, int stack_level, bytecode_list * code,
                        int * result)
{
    int v = 0;
    bytecode bc = { 0 };

    if (value->call.vars)
    {
        expr_list_emit(value->call.vars, stack_level + v, code, result);
        v += value->call.vars->count;
    }
    expr_emit(value->call.func_expr, stack_level + v, code, result);

    bc.type = BYTECODE_SLIDE;
    bc.slide.q = stack_level + v;
    bc.slide.m = v + 1;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_CALL;
    bytecode_add(code, &bc);

    return 0;
}

int expr_func_emit(func * func_value, int stack_level, bytecode_list * code,
                   int * result)
{
    func_emit(func_value, stack_level, code, result);

    return 0;
}

int expr_emit(expr * value, int stack_level, bytecode_list * code, int * result)
{
    bytecode bc = { 0 };

    switch (value->type)
    {
    case EXPR_INT:
        expr_int_emit(value, stack_level, code, result);
        break;
    case EXPR_FLOAT:
        expr_float_emit(value, stack_level, code, result);
        break;
    case EXPR_ID:
        expr_id_emit(value, stack_level, code, result);
        break;
    case EXPR_NEG:
        expr_emit(value->left, stack_level, code, result);

        if (value->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_OP_NEG_INT;
            bytecode_add(code, &bc);
        }
        else if (value->comb == COMB_TYPE_FLOAT)
        {
            bc.type = BYTECODE_OP_NEG_FLOAT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no, "cannot neg type %s\n",
                            comb_type_str(value->comb));
        }
        break;
    case EXPR_ADD:
        expr_emit(value->left, stack_level, code, result);
        expr_emit(value->right, stack_level + 1, code, result);

        if (value->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_OP_ADD_INT;
            bytecode_add(code, &bc);
        }
        else if (value->comb == COMB_TYPE_FLOAT)
        {
            bc.type = BYTECODE_OP_ADD_FLOAT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no, "cannot add type %s\n",
                            comb_type_str(value->comb));
        }
        break;
    case EXPR_SUB:
        expr_emit(value->left, stack_level, code, result);
        expr_emit(value->right, stack_level + 1, code, result);

        if (value->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_OP_SUB_INT;
            bytecode_add(code, &bc);
        }
        else if (value->comb == COMB_TYPE_FLOAT)
        {
            bc.type = BYTECODE_OP_SUB_FLOAT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no, "cannot sub type %s\n",
                            comb_type_str(value->comb));
        }
        break;
    case EXPR_MUL:
        expr_emit(value->left, stack_level, code, result);
        expr_emit(value->right, stack_level + 1, code, result);

        if (value->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_OP_MUL_INT;
            bytecode_add(code, &bc);
        }
        else if (value->comb == COMB_TYPE_FLOAT)
        {
            bc.type = BYTECODE_OP_MUL_FLOAT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no, "cannot mul type %s\n",
                            comb_type_str(value->comb));
        }
        break;
    case EXPR_DIV:
        expr_emit(value->left, stack_level, code, result);
        expr_emit(value->right, stack_level + 1, code, result);

        bc.type = BYTECODE_LINE;
        bc.line.no = value->line_no;
        bytecode_add(code, &bc);

        if (value->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_OP_DIV_INT;
            bytecode_add(code, &bc);
        }
        else if (value->comb == COMB_TYPE_FLOAT)
        {
            bc.type = BYTECODE_OP_DIV_FLOAT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no, "cannot div type %s\n",
                            comb_type_str(value->comb));
        }
        break;
    case EXPR_MOD:
        expr_emit(value->right, stack_level, code, result);
        expr_emit(value->left, stack_level + 1, code, result);

        bc.type = BYTECODE_LINE;
        bc.line.no = value->line_no;
        bytecode_add(code, &bc);

        if (value->left->comb == COMB_TYPE_INT &&
            value->right->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_OP_MOD_INT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no, "cannot mod type %s %s\n",
                            comb_type_str(value->left->comb),
                            comb_type_str(value->right->comb));
        }
        break;
    case EXPR_LT:
        expr_emit(value->left, stack_level, code, result);
        expr_emit(value->right, stack_level + 1, code, result);

        if (value->left->comb == COMB_TYPE_INT &&
            value->right->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_OP_LT_INT;
            bytecode_add(code, &bc);
        }
        else if (value->left->comb == COMB_TYPE_FLOAT &&
                 value->right->comb == COMB_TYPE_FLOAT)
        {
            bc.type = BYTECODE_OP_LT_FLOAT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no, "cannot lt different types %s %s\n",
                            comb_type_str(value->left->comb),
                            comb_type_str(value->right->comb));
        }
        break;
    case EXPR_GT:
        expr_emit(value->left, stack_level, code, result);
        expr_emit(value->right, stack_level + 1, code, result);

        if (value->left->comb == COMB_TYPE_INT &&
            value->right->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_OP_GT_INT;
            bytecode_add(code, &bc);
        }
        else if (value->left->comb == COMB_TYPE_FLOAT &&
                 value->right->comb == COMB_TYPE_FLOAT)
        {
            bc.type = BYTECODE_OP_GT_FLOAT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no, "cannot gt different types %s %s\n",
                            comb_type_str(value->left->comb),
                            comb_type_str(value->right->comb));
        }
        break;
    case EXPR_LTE:
        expr_emit(value->left, stack_level, code, result);
        expr_emit(value->right, stack_level + 1, code, result);

        if (value->left->comb == COMB_TYPE_INT &&
            value->right->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_OP_LTE_INT;
            bytecode_add(code, &bc);
        }
        else if (value->left->comb == COMB_TYPE_FLOAT &&
                 value->right->comb == COMB_TYPE_FLOAT)
        {
            bc.type = BYTECODE_OP_LTE_FLOAT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no,
                            "cannot lte different types %s %s\n",
                            comb_type_str(value->left->comb),
                            comb_type_str(value->right->comb));
        }
        break;
    case EXPR_GTE:
        expr_emit(value->left, stack_level, code, result);
        expr_emit(value->right, stack_level + 1, code, result);

        if (value->left->comb == COMB_TYPE_INT &&
            value->right->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_OP_GTE_INT;
            bytecode_add(code, &bc);
        }
        else if (value->left->comb == COMB_TYPE_FLOAT &&
                 value->right->comb == COMB_TYPE_FLOAT)
        {
            bc.type = BYTECODE_OP_GTE_FLOAT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no,
                            "cannot gte different types %s %s\n",
                            comb_type_str(value->left->comb),
                            comb_type_str(value->right->comb));
        }
        break;
    case EXPR_EQ:
        expr_emit(value->left, stack_level, code, result);
        expr_emit(value->right, stack_level + 1, code, result);

        if (value->left->comb == COMB_TYPE_INT &&
            value->right->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_OP_EQ_INT;
            bytecode_add(code, &bc);
        }
        else if (value->left->comb == COMB_TYPE_FLOAT &&
                 value->right->comb == COMB_TYPE_FLOAT)
        {
            bc.type = BYTECODE_OP_EQ_FLOAT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no, "cannot eq different types %s %s\n",
                            comb_type_str(value->left->comb),
                            comb_type_str(value->right->comb));
        }
        break;
    case EXPR_NEQ:
        expr_emit(value->left, stack_level, code, result);
        expr_emit(value->right, stack_level + 1, code, result);

        if (value->left->comb == COMB_TYPE_INT &&
            value->right->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_OP_NEQ_INT;
            bytecode_add(code, &bc);
        }
        else if (value->left->comb == COMB_TYPE_FLOAT &&
                 value->right->comb == COMB_TYPE_FLOAT)
        {
            bc.type = BYTECODE_OP_NEQ_FLOAT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no,
                            "cannot neq different types %s %s\n",
                            comb_type_str(value->left->comb),
                            comb_type_str(value->right->comb));
        }

        break;
    case EXPR_AND:
        expr_and_emit(value, stack_level, code, result);
        break;
    case EXPR_OR:
        expr_or_emit(value, stack_level, code, result);
        break;
    case EXPR_NOT:
        expr_not_emit(value, stack_level, code, result);
        break;
    case EXPR_SUP:
        expr_emit(value->left, stack_level, code, result);
        break;
    case EXPR_COND:
        expr_cond_emit(value, stack_level, code, result);
        break;
    case EXPR_CALL:
        expr_call_emit(value, stack_level, code, result);
        break;
    case EXPR_LAST_CALL:
        expr_last_call_emit(value, stack_level, code, result);
        break;
    case EXPR_FUNC:
        if (value->func_value)
        {
            expr_func_emit(value->func_value, 1, code, result);
        }
        break;
    case EXPR_BUILD_IN:
        expr_list_emit(value->func_build_in.param, stack_level, code, result);

        bc.type = BYTECODE_BUILD_IN;
        bc.build_in.id = value->func_build_in.id;
        bytecode_add(code, &bc);
        break;
    case EXPR_INT_TO_FLOAT:
        expr_emit(value->left, stack_level, code, result);

        if (value->left->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_INT_TO_FLOAT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no, "cannot convert type %s to float\n",
                            comb_type_str(value->left->comb));
        }

        break;
    case EXPR_FLOAT_TO_INT:
        expr_emit(value->left, stack_level, code, result);

        if (value->left->comb == COMB_TYPE_FLOAT)
        {
            bc.type = BYTECODE_FLOAT_TO_INT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no, "cannot convert type %s to int\n",
                            comb_type_str(value->left->comb));
        }
        break;
    }
    return 0;
}

int expr_list_emit(expr_list * list, int stack_level, bytecode_list * code,
                   int * result)
{
    int e = 0;
    expr_list_node * node = list->head;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            expr_emit(value, stack_level + e++, code, result);
        }
        node = node->prev;
    }

    return 0;
}

int func_emit(func * func_value, int stack_level, bytecode_list * code,
              int * result)
{
    int var_count = 0;
    int freevar_count = 0;
    int func_count = 0;
    bytecode bc = { 0 };
    bytecode *jump, *labelA, *labelB;

    bc.type = BYTECODE_FUNC_DEF;
    bytecode_add(code, &bc);

    if (func_value->line_no > 0)
    {
        bc.type = BYTECODE_LINE;
        bc.line.no = func_value->line_no;
        bytecode_add(code, &bc);
    }

    if (func_value->freevars != NULL)
    {
        func_freevar_list_emit(func_value->freevars, stack_level, code, result);
        freevar_count = func_value->freevars->count;
    }

    bc.type = BYTECODE_GLOBAL_VEC;
    bc.global_vec.count = freevar_count;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_ID_FUNC_FUNC;
    bc.id_func.func_value = func_value;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_JUMP;
    jump = bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    labelA = bytecode_add(code, &bc);
    func_value->addr = labelA->addr;

    if (func_value->body && func_value->body->funcs)
    {
        func_list_emit(func_value->body->funcs, 0, code, result);
        func_count = func_value->body->funcs->count;
    }

    if (func_value->body && func_value->body->ret)
    {
        expr_emit(func_value->body->ret, func_count, code, result);
    }

    if (bc.line.no > 0)
    {
        bc.type = BYTECODE_LINE;
        bc.line.no = func_value->body->ret->line_no;
        bytecode_add(code, &bc);
    }

    if (func_value->vars != NULL)
    {
        var_count = func_value->vars->count;
    }

    bc.type = BYTECODE_RET;
    bc.ret.count = var_count;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    labelB = bytecode_add(code, &bc);
    jump->jump.offset = labelB->addr - jump->addr;

    return 0;
}

int func_main_emit(never * nev, int stack_level, bytecode_list * code,
                   int * result)
{
    symtab_entry * entry = NULL;

    entry = symtab_lookup(nev->stab, "main", SYMTAB_FLAT);
    if (entry != NULL && entry->type == SYMTAB_FUNC)
    {
        bytecode bc = { 0 };
        bytecode *mark, *label;

        bc.type = BYTECODE_MARK;
        mark = bytecode_add(code, &bc);

        bc.type = BYTECODE_PUSH_PARAM;
        bytecode_add(code, &bc);

        bc.type = BYTECODE_GLOBAL_VEC;
        bc.global_vec.count = 0;
        bytecode_add(code, &bc);

        bc.type = BYTECODE_ID_FUNC_FUNC;
        bc.id_func.func_value = entry->func_value;
        bytecode_add(code, &bc);

        bc.type = BYTECODE_CALL;
        bytecode_add(code, &bc);

        bc.type = BYTECODE_LABEL;
        label = bytecode_add(code, &bc);
        mark->mark.addr = label->addr;

        bc.type = BYTECODE_HALT;
        label = bytecode_add(code, &bc);
        mark->mark.addr = label->addr;
    }
    else
    {
        *result = GENCODE_FAIL;
        print_error_msg(0, "no main function defined\n");
    }
    return 0;
}

int func_list_emit(func_list * list, int stack_level, bytecode_list * code,
                   int * result)
{
    bytecode bc = { 0 };
    unsigned int n = list->count;

    bc.type = BYTECODE_ALLOC;
    bc.alloc.n = n;
    bytecode_add(code, &bc);

    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value != NULL)
        {
            func_emit(func_value, stack_level + list->count, code, result);

            bc.type = BYTECODE_REWRITE;
            bc.rewrite.j = n--;
            bytecode_add(code, &bc);
        }
        node = node->next;
    }
    return 0;
}

int never_emit(never * nev, bytecode_list * code)
{
    int stack_level = 0;
    int gencode_res = 0;

    if (nev->funcs)
    {
        func_list_emit(nev->funcs, stack_level, code, &gencode_res);
        func_main_emit(nev, stack_level, code, &gencode_res);
    }

    return gencode_res;
}

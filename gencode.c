#include <stdio.h>
#include "gencode.h"

int expr_gencode(symtab * tab, expr * value, int * result)
{
    switch (value->type)
    {
        case EXPR_INT:
            printf("gencode EXPR_INT %d\n", value->int_value);
        break;
        case EXPR_ID:
            printf("gencode EXPR_ID %s %s\n", value->id, comb_type_str(value->comb));
        break;
        case EXPR_NEG:
            expr_gencode(tab, value->left, result);
        break;
        case EXPR_ADD:
        case EXPR_SUB:
        case EXPR_MUL:
        case EXPR_DIV:
        case EXPR_LT:
        case EXPR_GT:
        case EXPR_LTE:
        case EXPR_GTE:
        case EXPR_EQ:
            expr_gencode(tab, value->left, result);
            expr_gencode(tab, value->right, result);
        break;
        break;
        case EXPR_SUP:
            expr_gencode(tab, value->left, result);
        break;
        case EXPR_COND:
            expr_gencode(tab, value->left, result);
            expr_gencode(tab, value->middle, result);
            expr_gencode(tab, value->right, result);
        break;
        case EXPR_CALL:
            expr_gencode(tab, value->func_expr, result);
            expr_list_gencode(tab, value->vars, result);
        break;
        case EXPR_FUNC:
        {
            if (value->func_value)
            {
                func_gencode(value->func_value->stab, value->func_value, result);
            }
        }
        break;
    }
    return 0;
}

int expr_list_gencode(symtab * tab, expr_list * list, int * result)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value)
        {
            expr_gencode(tab, value, result);
        }
        node = node->next;
    }
    return 0;
}

int func_gencode(symtab * tab, func * func_value, int * result)
{
    if (func_value->body && func_value->body->funcs)
    {
        func_list_gencode(tab, func_value->body->funcs, result);
    }
    if (func_value->body && func_value->body->ret)
    {
        expr_gencode(tab, func_value->body->ret, result);
    }
    return 0;
}

int func_list_gencode(symtab * tab, func_list * list, int * result)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value)
        {
            func_gencode(func_value->stab, func_value, result);
        }
        node = node->next;
    }
    return 0;
}

int never_gencode(never * nev)
{
    int gencode_res = 0;

    if (nev->funcs)
    {
        func_list_gencode(nev->stab, nev->funcs, &gencode_res);
    }
    
    return gencode_res;
}





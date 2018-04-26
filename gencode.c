#include <stdio.h>
#include <assert.h>
#include "gencode.h"
#include "symtab.h"

int expr_id_gencode(unsigned int syn_level, func * func_value, expr * value, int * result)
{
    symtab_entry * entry = NULL;
    
    printf("gencode EXPR_ID %s %s %d\n", value->id, comb_type_str(value->comb), syn_level);

    entry = symtab_lookup(func_value->stab, value->id, SYMTAB_NESTED);
    if (entry != NULL && entry->var_func_value)
    {
        symtab_entry_print(entry);
        
        if (entry->type == SYMTAB_FUNC)
        {
            func * func_value = (func *)entry->var_func_value;
            if (func_value)
            {
                /* function defined */
                /* function ip_addr */
            }
        }
        else if (entry->type == SYMTAB_VAR)
        {
            var * var_value = (var *)entry->var_func_value;
            if (var_value->type == VAR_INT)
            {
                /* integer passed as variable */
                /* if syn_level == entry->syn_level is local */
                /* if syn level != entry->syn_level is global, add to function globals */
            }
            else if (var_value->type == VAR_FUNC)
            {
                /* function passed as variable */
                /* if syn_level == entry->syn_level is local */
                /* if syn level != entry->syn_level is global, add to function globals */
            }
        }
    }
    else
    {                    
        *result = 1;
        printf("cannot find variable %s, at this stage it is very bad\n", value->id);
        assert(0);
    }
    
    return 0;
}

int expr_gencode(unsigned int syn_level, func * func_value, expr * value, int * result)
{
    switch (value->type)
    {
        case EXPR_INT:
            printf("gencode EXPR_INT %d\n", value->int_value);
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
        case EXPR_LT:
        case EXPR_GT:
        case EXPR_LTE:
        case EXPR_GTE:
        case EXPR_EQ:
            expr_gencode(syn_level, func_value, value->left, result);
            expr_gencode(syn_level, func_value, value->right, result);
        break;
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
            expr_gencode(syn_level, func_value, value->func_expr, result);
            expr_list_gencode(syn_level, func_value, value->vars, result);
        break;
        case EXPR_FUNC:
        {
            if (value->func_value)
            {
                func_gencode(syn_level + 1, value->func_value, result);
            }
        }
        break;
    }
    return 0;
}

int expr_list_gencode(unsigned int syn_level, func * func_value, expr_list * list, int * result)
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
            value->index = index++;
        }
        node = node->next;
    }
    return 0;
}

int func_gencode(unsigned int syn_level, func * func_value, int * result)
{
    if (func_value != NULL && func_value->stab != NULL)
    {
        symtab_set_syn_level(func_value->stab, syn_level);
    }
    if (func_value != NULL && func_value->vars != NULL)
    {
        func_enum_vars(func_value);
    }

    if (func_value->body && func_value->body->funcs)
    {
        func_list_gencode(syn_level + 1, func_value->body->funcs, result);
    }
    if (func_value->body && func_value->body->ret)
    {
        expr_gencode(syn_level, func_value, func_value->body->ret, result);
    }
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
            func_gencode(syn_level, func_value, result);
        }
        node = node->next;
    }
    return 0;
}

int never_gencode(never * nev)
{
    int gencode_res = 0;
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





#include <stdio.h>
#include "typecheck.h"
#include "symtab.h"

/*
 * check types
 */
int expr_check_type(symtab * tab, expr * value, int * result)
{
    switch (value->type)
    {
        case EXPR_INT:
            value->comb = COMB_TYPE_INT;
        break;
        case EXPR_ID:
        {
            symtab_entry * entry = NULL;
            
            entry = symtab_lookup_func(tab, value->id, SYMTAB_NESTED);
            if (entry != NULL)
            {
                func * func_value = (func *)entry->arg_func_value;
            
                value->comb = COMB_TYPE_FUNC;
                value->comb_args = func_value->args;
                value->comb_ret = func_value->ret;
            }
            else
            {
                entry = symtab_lookup_arg(tab, value->id, SYMTAB_NESTED);
                if (entry != NULL)
                {
                    if (entry->type == SYMTAB_ARG && entry->arg_func_value)
                    {
                        arg * arg_value = (arg *)entry->arg_func_value;
                        if (arg_value->type == ARG_INT)
                        {
                            value->comb = COMB_TYPE_INT;
                        }
                        else if (arg_value->type == ARG_FUNC)
                        {
                            value->comb = COMB_TYPE_FUNC;
                            value->comb_args = arg_value->args;
                            value->comb_ret = arg_value->ret;
                        }
                    }
                }
                else
                {
                    printf("cannot find variable %s\n", value->id);
                }
            }
        }
        break;
        case EXPR_NEG:
        {
            expr_check_type(tab, value->left, result);
            if (value->left->comb == COMB_TYPE_INT)
            {
                value->comb = COMB_TYPE_INT;
            }
            else
            {
                *result = TYPECHECK_FAIL;
                value->comb = COMB_TYPE_ERR;
                printf("cannot negate type %d\n", value->left->comb);
            }
        }
        break;
        case EXPR_ADD:
        case EXPR_SUB:
        case EXPR_MUL:
        case EXPR_DIV:
        {
            expr_check_type(tab, value->left, result);
            expr_check_type(tab, value->right, result);
            if (value->left->comb == COMB_TYPE_INT &&
                value->right->comb == COMB_TYPE_INT)
            {
                value->comb = COMB_TYPE_INT;
            }
            else
            {
                *result = TYPECHECK_FAIL;
                value->comb = COMB_TYPE_ERR;
                printf("cannot exec arithmetic operation on types %d %d\n",
                       value->left->comb, value->right->comb);
            }
        }
        break;
        case EXPR_LT:
        case EXPR_GT:
        case EXPR_LTE:
        case EXPR_GTE:
        {
            expr_check_type(tab, value->left, result);
            expr_check_type(tab, value->right, result);
            if (value->left->comb == COMB_TYPE_INT &&
                value->left->comb == COMB_TYPE_INT)
            {
                value->comb = COMB_TYPE_BOOL;
            }
            else
            {
                *result = TYPECHECK_FAIL;
                value->comb = COMB_TYPE_ERR;
                printf("cannot compare not integers %d %d\n",
                       value->left->comb, value->right->comb);
            }
        }
        break;
        case EXPR_EQ:
            expr_check_type(tab, value->left, result);
            expr_check_type(tab, value->right, result);
            if (value->left->comb == COMB_TYPE_INT &&
                value->right->comb == COMB_TYPE_INT)
            {
                value->comb = COMB_TYPE_BOOL;
            }
            else if (value->left->comb == COMB_TYPE_BOOL &&
                     value->right->comb == COMB_TYPE_BOOL)
            {
                value->comb = COMB_TYPE_BOOL;
            }
            else
            {
                *result = TYPECHECK_FAIL;
                value->comb = COMB_TYPE_ERR;
                printf("cannot equal types %d %d\n",
                       value->left->comb, value->right->comb);
            }
        break;
        case EXPR_SUP:
            expr_check_type(tab, value->left, result);
        break;
        case EXPR_COND:
        {
            expr_check_type(tab, value->left, result);
            expr_check_type(tab, value->middle, result);
            expr_check_type(tab, value->right, result);
            
            if (value->left->comb == COMB_TYPE_INT ||
                value->left->comb == COMB_TYPE_BOOL)
            {
                if (value->middle->comb == value->right->comb)
                {
                    if (value->middle->comb == COMB_TYPE_FUNC)
                    {
                    }
                    else
                    {
                        value->comb = value->middle->comb;
                    }
                }
                else
                {
                    printf("types on conditional expression do not match %d %d\n",
                           value->middle->comb, value->right->comb);
                }
            }
            else
            {
                *result = TYPECHECK_FAIL;
                value->comb = COMB_TYPE_ERR;
                printf("cannot execute conditional operator on %d\n",
                       value->left->comb);
            }
        }
        break;
        case EXPR_CALL:
        {
            symtab_entry * entry = NULL;
            
            entry = symtab_lookup_func(tab, value->func_id, SYMTAB_NESTED);
            if (entry != NULL)
            {
                func * func_value = (func *)entry->arg_func_value;
                if (func_value)
                {
                    
                }
            }
            else
            {
                entry = symtab_lookup_arg(tab, value->func_id, SYMTAB_NESTED);
                if (entry != NULL)
                {
                }
                else
                {
                    printf("cannot find function %s\n", value->func_id);
                }
            }
        }
        break;
        case EXPR_FUNC:
        {
            func_check_type(value->func_value->stab, value->func_value, result);
            
            value->comb = COMB_TYPE_FUNC;
            value->comb_args = value->func_value->args;
            value->comb_ret = value->func_value->ret;
        }
        break;
    }
    return 0;
}

int expr_list_check_type(symtab * tab, expr_list * list, int * result)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            expr_check_type(tab, value, result);
        }
        node = node->next;
    }

    return 0;
}

int func_check_type(symtab * tab, func * func_value, int * result)
{
    if (func_value->body && func_value->body->funcs)
    {
        func_list_check_type(tab, func_value->body->funcs, result);
    }
    if (func_value->body && func_value->body->ret)
    {
        expr_check_type(tab, func_value->body->ret, result);
    }

    return 0;
}

int func_list_check_type(symtab * tab, func_list * list, int * result)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value != NULL)
        {
            func_check_type(func_value->stab, func_value, result);
        }
        node = node->next;
    }

    return 0;
}

int never_check_type(never * nev, int * result)
{
    func_list_check_type(nev->stab, nev->funcs, result);

    return 0;
}

/*
 * Check function call types
 */
int expr_call_check_call(symtab * tab, expr * value, int * result)
{
    symtab_entry * entry = symtab_lookup_func(tab, value->func_id, SYMTAB_NESTED);
    if (entry == NULL)
    {
        *result = TYPECHECK_FAIL;
        printf("cannot find function %s\n", value->func_id);

        return 0;
    }
    
    if (entry->type == SYMTAB_FUNC)
    {
        func * func_value = entry->arg_func_value;
        if (func_value && func_value->args && value->args &&
            func_value->args->count != value->args->count)
        {
            *result = TYPECHECK_FAIL;
            printf("improper number of %s parameters, got %d but expected %d\n",
                    value->func_id, value->args->count, func_value->args->count);
        }
    }
    else
    {
        *result = TYPECHECK_FAIL;
        printf("found variable but function expected %s\n", value->func_id);
    }
    return 0;
}
 
int expr_check_call(symtab * tab, expr * value, int * result)
{
    switch (value->type)
    {
        case EXPR_INT:
        /* is not possible */
        break;
        case EXPR_ID:
        /* is not possible */
        break;
        case EXPR_NEG:
            expr_check_call(tab, value->left, result);
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
            expr_check_call(tab, value->left, result);
            expr_check_call(tab, value->right, result);
        break;
        case EXPR_SUP:
            expr_check_call(tab, value->left, result);
        break;
        case EXPR_COND:
            expr_check_call(tab, value->left, result);
            expr_check_call(tab, value->middle, result);
            expr_check_call(tab, value->right, result);
        break;
        case EXPR_CALL:
            expr_call_check_call(tab, value, result);
        break;
        case EXPR_FUNC:
            func_check_call(value->func_value->stab, value->func_value, result);
        break;
    }
    return 0;
}

int expr_list_check_call(symtab * tab, expr_list * list, int * result)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            expr_check_call(tab, value, result);
        }    
        node = node->next;
    }
    
    return 0;
}

int func_check_call(symtab * tab, func * func_value, int * result)
{
    if (func_value->body && func_value->body->funcs)
    {
        func_list_check_call(tab, func_value->body->funcs, result);
    }
    if (func_value->body && func_value->body->ret)
    {
        expr_check_call(tab, func_value->body->ret, result);
    }

    return 0;
}

int func_list_check_call(symtab * tab, func_list * list, int * result)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value != NULL)
        {
            func_check_call(func_value->stab, func_value, result);
        }
    
        node = node->next;
    }

    return 0;
}

int never_check_func_call(never * nev, int * result)
{
    func_list_check_call(nev->stab, nev->funcs, result);

    return 0;
}

/*
 * Check undefined IDs
 */
int expr_check_undefined_ids(symtab * tab, expr * value, int * result)
{
    switch (value->type)
    {
        case EXPR_INT:
        /* not possible */
        break;
        case EXPR_ID:
        {
            printf("\tsearching id %s\n", value->id);
            symtab_entry * entry = symtab_lookup_arg(tab, value->id, SYMTAB_NESTED);
            if (entry == NULL)
            {
                *result = TYPECHECK_FAIL;
                printf("\tcannot find variable %s\n", value->id);
            }
        }
        break;
        case EXPR_NEG:
            expr_check_undefined_ids(tab, value->left, result);
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
            expr_check_undefined_ids(tab, value->left, result);
            expr_check_undefined_ids(tab, value->right, result);
        break;
        case EXPR_SUP:
            expr_check_undefined_ids(tab, value->left, result);
        break;
        case EXPR_COND:
            expr_check_undefined_ids(tab, value->left, result);
            expr_check_undefined_ids(tab, value->middle, result);
            expr_check_undefined_ids(tab, value->right, result);
        break;
        case EXPR_CALL:
        {
            symtab_entry * entry = symtab_lookup_func(tab, value->id, SYMTAB_NESTED);
            if (entry == NULL)
            {
                *result = TYPECHECK_FAIL;
                printf("\tcannot find function %s\n", value->id);
            }
            else if (entry != NULL && entry->type == SYMTAB_ARG)
            {
                *result = TYPECHECK_FAIL;
                printf("\texpected function but variable %s found\n", value->id);
            }
            else
            {
                printf("\tfound function %s\n", value->id);
                expr_list_check_undefined_ids(tab, value->args, result);
            }
        }
        break;
        case EXPR_FUNC:
            func_check_undefined_ids(value->func_value->stab, value->func_value, result);
        break;
    }
    return 0;
}

int expr_list_check_undefined_ids(symtab * tab, expr_list * list, int * result)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            expr_check_undefined_ids(tab, value, result);
        }            
        node = node->next;
    }
    return 0;
}

int func_check_undefined_ids(symtab * tab, func * func_value, int * result)
{
    printf("verifing %s\n", func_value->id);

    if (func_value->body && func_value->body->funcs)
    {
        func_list_check_undefined_ids(tab, func_value->body->funcs, result);
    }
    if (func_value->body && func_value->body->ret)
    {
        expr_check_undefined_ids(tab, func_value->body->ret, result);
    }
    
    return 0;
}

int func_list_check_undefined_ids(symtab * tab, func_list * list, int * result)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {   
        func * func_value = node->value;
        if (func_value)
        {
            func_check_undefined_ids(func_value->stab, func_value, result);
        }    
        node = node->next;
    }

    return 0;
}

int never_check_undefined_ids(never * nev, int * result)
{
    func_list_check_undefined_ids(nev->stab, nev->funcs, result);

    return 0;
}

/*
 * Add symbols to symtab
 */
int symtab_add_arg_from_arg_list(symtab * tab, arg_list * list, int * result)
{
    arg_list_node * node = list->tail;
    while (node != NULL)
    {
        arg * arg_value = node->value;
        if (arg_value && arg_value->id)
        {
            symtab_entry * entry = symtab_lookup_arg(tab, arg_value->id, SYMTAB_FLAT);
            if (entry == NULL)
            {
                symtab_add_arg(tab, arg_value);
            }
            else
            {
                *result = TYPECHECK_FAIL;
                if (entry->type == SYMTAB_FUNC)
                {
                    printf("function %s alread defined\n", entry->id);
                }
                else if (entry->type == SYMTAB_ARG)
                {
                    printf("parameter %s already defined\n", entry->id);
                }
            }
        }
        node = node->next;
    }
    return 0;
}

int symtab_add_func_from_func_list(symtab * tab, func_list * list, int * result)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value && func_value->id)
        {
            symtab_entry * entry = symtab_lookup_func(tab, func_value->id, SYMTAB_FLAT);
            if (entry == NULL)
            {
                symtab_add_func(tab, func_value);
            }
            else
            {
                *result = TYPECHECK_FAIL;
                if (entry->type == SYMTAB_FUNC)
                {
                    printf("function %s alread defined\n", entry->id);
                }
                else if (entry->type == SYMTAB_ARG)
                {
                    printf("parameter %s already defined\n", entry->id);
                }
            }
        }
        node = node->next;
    }
    return 0;
}

int symtab_add_entry_expr(symtab * stab, expr * value, int * result)
{
    switch (value->type)
    {
        case EXPR_INT:
        break;
        case EXPR_ID:
        break;
        case EXPR_NEG:
            symtab_add_entry_expr(stab, value->left, result);
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
            symtab_add_entry_expr(stab, value->left, result);
            symtab_add_entry_expr(stab, value->right, result);
        break;
        case EXPR_SUP:
            symtab_add_entry_expr(stab, value->left, result);
        break;
        case EXPR_COND:
            symtab_add_entry_expr(stab, value->left, result);
            symtab_add_entry_expr(stab, value->middle, result);
            symtab_add_entry_expr(stab, value->right, result);
        break;
        case EXPR_CALL:
            /* not possible to find function definition here */
        break;
        case EXPR_FUNC:
            symtab_add_entry_func(stab, value->func_value, result);
        break;
    }
    return 0;
}

int symtab_add_entry_func(symtab * stab_parent, func * func_value, int * result)
{
    if (func_value->stab == NULL)
    {
        func_value->stab = symtab_new(32, stab_parent);
    }
    if (func_value->args)
    {
        symtab_add_arg_from_arg_list(func_value->stab, func_value->args, result);
    }
    if (func_value->body && func_value->body->funcs)
    {
        symtab_add_func_from_func_list(func_value->stab, func_value->body->funcs, result);
    }
    if (func_value->body && func_value->body->funcs)
    {
        symtab_add_entry_func_list(func_value->stab, func_value->body->funcs, result);
    }
    if (func_value->body && func_value->body->ret)
    {
        symtab_add_entry_expr(func_value->stab, func_value->body->ret, result);
    }

    return 0;
}

int symtab_add_entry_func_list(symtab * stab_parent, func_list * list, int * result)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value)
        {
            symtab_add_entry_func(stab_parent, func_value, result);
        }   
        node = node->next;
    }

    return 0;
}

int symtab_add_entry_never(never * nev, int * result)
{
    if (nev->stab == NULL)
    {
        nev->stab = symtab_new(32, NULL);
    }
    
    symtab_add_func_from_func_list(nev->stab, nev->funcs, result);
    symtab_add_entry_func_list(nev->stab, nev->funcs, result);
    
    return 0;
}

/**
 * print symtabs
 */
int print_symtabs_expr(expr * value)
{
    switch (value->type)
    {
        case EXPR_INT:
            /* no symtabs possible */
        break;
        case EXPR_ID:
            /* no symtabs possible */
        break;
        case EXPR_NEG:
            print_symtabs_expr(value->left);
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
            print_symtabs_expr(value->left);
            print_symtabs_expr(value->right);
        break;
        case EXPR_SUP:
            print_symtabs_expr(value->left);
        break;
        case EXPR_COND:
            print_symtabs_expr(value->left);
            print_symtabs_expr(value->middle);
            print_symtabs_expr(value->right);
        break;
        case EXPR_CALL:
            /* no symtabs possible */
        break;
        case EXPR_FUNC:
            print_symtabs_func(value->func_value);
        break;
    }
    return 0;
}

int print_symtabs_func(func * func_value)
{
    if (func_value->id)
    {
        printf("function: %s\n", func_value->id);
    }
    if (func_value->stab)
    {
        symtab_print(func_value->stab);
    }
    if (func_value->body && func_value->body->funcs)
    {
        print_symtabs_func_list(func_value->body->funcs);
    }
    if (func_value->body && func_value->body->ret)
    {
        print_symtabs_expr(func_value->body->ret);
    }
    return 0;
}

int print_symtabs_func_list(func_list * list)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value)
        {
            print_symtabs_func(func_value);
        }
        node = node->next;
    }
    return 0;
}

int print_symtabs(never * nev)
{
    if (nev->stab)
    {
        symtab_print(nev->stab);
    }
    print_symtabs_func_list(nev->funcs);
        
    return 0;
}

/**
 * print functions
 */
int print_func_list(func_list * list, int depth)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value && func_value->id)
        {
            printf("function (%d): %s\n", depth, func_value->id);
        }
        if (func_value && func_value->body && func_value->body->funcs)
        {
            print_func_list(func_value->body->funcs, depth + 1);
        }
        node = node->next;
    }
    return 0;
}

int print_functions(never * nev)
{
    if (nev->funcs)
    {
        print_func_list(nev->funcs, 1);
    }
    return 0;
}



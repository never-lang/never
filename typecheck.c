#include <stdio.h>
#include "typecheck.h"
#include "symtab.h"

int expr_set_return_type(expr * value, arg * ret)
{
    if (ret == NULL)
    {
        value->comb = COMB_TYPE_VOID;
    }
    else if (ret->type == ARG_INT)
    {
        value->comb = COMB_TYPE_INT;
    }
    else if (ret->type == ARG_FUNC)
    {
        value->comb = COMB_TYPE_FUNC;
        value->comb_args = ret->args;
        value->comb_ret = ret->ret;
    }
    else
    {
        value->comb = COMB_TYPE_ERR;
    }
    return 0;
}

/*
 * check types
 */
int arg_cmp(arg * arg_one, arg * arg_two)
{
    if (arg_one == NULL && arg_two == NULL)
    {
        return TYPECHECK_SUCC;
    }
    if ((arg_one == NULL && arg_two != NULL) || (arg_one != NULL && arg_two == NULL))
    {
        return TYPECHECK_FAIL;
    }
    if (arg_one->type == ARG_INT && arg_two->type == ARG_INT)
    {
        return TYPECHECK_SUCC;
    }
    else if (arg_one->type == ARG_FUNC && arg_two->type == ARG_FUNC)
    {
        return func_cmp(arg_one->args, arg_one->ret, arg_two->args, arg_one->ret);
    }
    else
    {
        return TYPECHECK_FAIL;
    }
}

int arg_list_cmp(arg_list * arg_one, arg_list * arg_two)
{
    if (arg_one == NULL && arg_two == NULL)
    {
        return TYPECHECK_SUCC;
    }
    if ((arg_one == NULL && arg_two != NULL) || (arg_one != NULL && arg_two == NULL))
    {
        return TYPECHECK_FAIL;
    }

    if (arg_one->count != arg_two->count)
    {
        return TYPECHECK_FAIL;
    }
    
    arg_list_node * arg_one_node = arg_one->tail;
    arg_list_node * arg_two_node = arg_two->tail;
    while (arg_one_node != NULL && arg_two_node != NULL)
    {
        arg * arg_one_value = arg_one_node->value;
        arg * arg_two_value = arg_two_node->value;
        
        if (arg_cmp(arg_one_value, arg_two_value) == TYPECHECK_FAIL)
        {
            return TYPECHECK_FAIL;
        }
        
        arg_one_node = arg_one_node->next;
        arg_two_node = arg_two_node->next;
    }

    return TYPECHECK_SUCC;
}

int func_cmp(arg_list * arg_list_one, arg * ret_one,
             arg_list * arg_list_two, arg * ret_two)
{
    if (arg_list_cmp(arg_list_one, arg_list_two) == TYPECHECK_SUCC &&
        arg_cmp(ret_one, ret_two) == TYPECHECK_SUCC)
    {
        return TYPECHECK_SUCC;
    }
    else
    {
        return TYPECHECK_FAIL;
    }
}

int arg_expr_cmp(arg * arg_value, expr * expr_value)
{

    if (arg_value == NULL && expr_value == NULL)
    {
        return TYPECHECK_SUCC;
    }
    if ((arg_value != NULL && expr_value == NULL) || (arg_value == NULL && expr_value != NULL))
    {
        return TYPECHECK_FAIL;
    }

    if (arg_value->type == ARG_INT && expr_value->comb == COMB_TYPE_INT)
    {
        return TYPECHECK_SUCC;
    }
    else if (arg_value->type == ARG_FUNC && expr_value->comb == COMB_TYPE_FUNC)
    {
        return func_cmp(arg_value->args, arg_value->ret,
                        expr_value->comb_args, expr_value->comb_ret);
    }
    else
    {
        return TYPECHECK_FAIL;
    }
}
 
int arg_expr_list_cmp(arg_list * args, expr_list * list)
{
    if (args == NULL && list == NULL)
    {
        return TYPECHECK_SUCC;
    }
    if ((args != NULL && list == NULL) || (args == NULL && list != NULL))
    {
        return TYPECHECK_FAIL;
    }
    if (args->count != list->count)
    {
        return TYPECHECK_FAIL;
    }
    
    arg_list_node * arg_node = args->tail;
    expr_list_node * expr_node = list->tail;
    while (arg_node != NULL && expr_node != NULL)
    {
        arg * arg_value = arg_node->value;
        expr * expr_value = expr_node->value;
        
        if (arg_expr_cmp(arg_value, expr_value) == TYPECHECK_FAIL)
        {
            return TYPECHECK_FAIL;
        }
        
        arg_node = arg_node->next;
        expr_node = expr_node->next;
    }

    return TYPECHECK_SUCC;
} 

int expr_id_check_type(symtab * tab, expr * value, int * result)
{
    symtab_entry * entry = NULL;

    entry = symtab_lookup(tab, value->id, SYMTAB_NESTED);
    if (entry != NULL && entry->arg_func_value)
    {
        if (entry->type == SYMTAB_FUNC)
        {
            func * func_value = (func *)entry->arg_func_value;
            
            value->comb = COMB_TYPE_FUNC;
            value->comb_args = func_value->args;
            value->comb_ret = func_value->ret;
        }
        else if (entry->type == SYMTAB_ARG)
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
        *result = TYPECHECK_FAIL;
        printf("cannot find variable %s\n", value->id);
    }
    return 0;
}

int expr_cond_check_type(symtab * tab, expr * value, int * result)
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
                 if (func_cmp(value->middle->comb_args, value->middle->comb_ret,
                              value->right->comb_args, value->right->comb_ret) == TYPECHECK_SUCC)
                 {
                     value->comb = COMB_TYPE_FUNC;
                     value->comb_args = value->middle->comb_args;
                     value->comb_ret = value->middle->comb_ret;
                 }
                 else
                 {
                     *result = TYPECHECK_FAIL;
                      printf("functions are different %s %s\n",
                             value->middle->id, value->right->id); 
                 }
             }
             else
             {
                 value->comb = value->middle->comb;
             }
         }
         else
         {
             printf("types on conditional expression do not match %s %s\n",
                     comb_type_str(value->middle->comb),
                     comb_type_str(value->right->comb));
         }
    }
    else
    {
         *result = TYPECHECK_FAIL;
         value->comb = COMB_TYPE_ERR;
         printf("cannot execute conditional operator on %s\n",
                 comb_type_str(value->left->comb));
    }
    return 0;
}

int expr_call_check_type(symtab * tab, expr * value, int * result)
{
    expr_check_type(tab, value->func_expr, result);
    if (value->args != NULL)
    {
        expr_list_check_type(tab, value->args, result);
    }

    switch (value->func_expr->comb)
    {
        case COMB_TYPE_FUNC:
            if (value->func_expr->comb_args->count != value->args->count)
            {
                *result = TYPECHECK_FAIL;
                 printf("improper number of parameters, got %d but expected %d\n",
                        value->args->count, value->func_expr->comb_args->count);
            }
            else
            { 
                if (arg_expr_list_cmp(value->func_expr->comb_args, value->args) == TYPECHECK_SUCC)
                {
                    expr_set_return_type(value, value->func_expr->comb_ret);
                }
                else
                {
                    *result = TYPECHECK_FAIL;
                    printf("function call type mismatch\n");
                }
            }            
        break;
        case COMB_TYPE_INT:
        case COMB_TYPE_UNKNOWN:
        case COMB_TYPE_ERR:
        case COMB_TYPE_VOID:
        case COMB_TYPE_BOOL:
            printf("cannot execute function on type %s\n", comb_type_str(value->comb));
        break;
    }

    return 0;
}

int expr_check_type(symtab * tab, expr * value, int * result)
{
    switch (value->type)
    {
        case EXPR_INT:
            value->comb = COMB_TYPE_INT;
        break;
        case EXPR_ID:
            expr_id_check_type(tab, value, result);
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
                printf("cannot negate type %s\n", comb_type_str(value->left->comb));
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
                printf("cannot exec arithmetic operation on types %s %s\n",
                       comb_type_str(value->left->comb),
                       comb_type_str(value->right->comb));
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
                printf("cannot equal types %s %s\n",
                       comb_type_str(value->left->comb),
                       comb_type_str(value->right->comb));
            }
        break;
        case EXPR_SUP:
            expr_check_type(tab, value->left, result);
            
            value->comb = value->left->comb;
            value->comb_args = value->left->comb_args;
            value->comb_ret = value->left->comb_ret;
        break;
        case EXPR_COND:
            expr_cond_check_type(tab, value, result);
        break;
        case EXPR_CALL:
            expr_call_check_type(tab, value, result);
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
        
        if (arg_expr_cmp(func_value->ret, func_value->body->ret) == TYPECHECK_FAIL)
        {
            *result = TYPECHECK_FAIL;
            printf("incorrect return type in function %s\n", func_value->id);
        }
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
            symtab_entry * entry = symtab_lookup(tab, arg_value->id, SYMTAB_FLAT);
            if (entry == NULL)
            {
                symtab_add_arg(tab, arg_value);
            }
            else
            {
                *result = TYPECHECK_FAIL;
                if (entry->type == SYMTAB_FUNC)
                {
                    printf("function %s already defined\n", entry->id);
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
            symtab_entry * entry = symtab_lookup(tab, func_value->id, SYMTAB_FLAT);
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
            symtab_add_entry_expr(stab, value->func_expr, result);
            if (value->args != NULL)
            {
                symtab_add_entry_expr_list(stab, value->args, result);
            }
        break;
        case EXPR_FUNC:
            symtab_add_entry_func(stab, value->func_value, result);
        break;
    }
    return 0;
}

int symtab_add_entry_expr_list(symtab * stab_parent, expr_list * list, int * result)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value)
        {
            symtab_add_entry_expr(stab_parent, value, result);
        }
        node = node->next;
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
            print_symtabs_expr(value->func_expr);
            if (value->args != NULL)
            {
                print_symtabs_expr_list(value->args);
            }
        break;
        case EXPR_FUNC:
            print_symtabs_func(value->func_value);
        break;
    }
    return 0;
}

int print_symtabs_expr_list(expr_list * list)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value)
        {
            print_symtabs_expr(value);
        }
        node = node->next;
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

int never_sem_check(never * nev)
{
    int typecheck_res;
    
    printf("---- add symbol table entries --- \n\n");
    typecheck_res = TYPECHECK_SUCC;
    symtab_add_entry_never(nev, &typecheck_res);
    
    print_functions(nev);
    print_symtabs(nev);

    printf("---- check types --- \n\n");
    typecheck_res = TYPECHECK_SUCC;
    never_check_type(nev, &typecheck_res);
    
    never_delete(nev);
    
    return 0;
}



#include <stdio.h>
#include "typecheck.h"
#include "symtab.h"

int expr_check_undefined_ids(symtab * tab, expr * value)
{
    switch (value->type)
    {
        case EXPR_INT:
        /* not possible */
        break;
        case EXPR_ID:
        {
            printf("\tsearching id %s\n", value->id);
            symtab_entry * entry = symtab_lookup_arg(tab, value->id);
            if (entry == NULL)
            {
                printf("\tcannot find variable %s\n", value->id);
            }
        }
        break;
        case EXPR_NEG:
            expr_check_undefined_ids(tab, value->left);
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
            expr_check_undefined_ids(tab, value->left);
            expr_check_undefined_ids(tab, value->right);
        break;
        case EXPR_SUP:
            expr_check_undefined_ids(tab, value->left);
        break;
        case EXPR_COND:
            expr_check_undefined_ids(tab, value->left);
            expr_check_undefined_ids(tab, value->middle);
            expr_check_undefined_ids(tab, value->right);
        break;
        case EXPR_CALL:
        {
            symtab_entry * entry = symtab_lookup_func(tab, value->id);
            if (entry == NULL)
            {
                printf("\tcannot find function %s\n", value->id);
            }
            else if (entry != NULL && entry->type == SYMTAB_ARG)
            {
                printf("\texpected function but variable %s found\n", value->id);
            }
            else
            {
                printf("\tfound function %s\n", value->id);
                expr_list_check_undefined_ids(tab, value->args);
            }
        }
        break;
        case EXPR_FUNC:
            func_check_undefined_ids(value->func_value->stab, value->func_value);
        break;
    }
    return 0;
}

int expr_list_check_undefined_ids(symtab * tab, expr_list * list)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            expr_check_undefined_ids(tab, value);
        }            
        node = node->next;
    }
    return 0;
}

int func_check_undefined_ids(symtab * tab, func * func_value)
{
    printf("verifing %s\n", func_value->id);

    if (func_value->body && func_value->body->funcs)
    {
        func_list_check_undefined_ids(tab, func_value->body->funcs);
    }
    if (func_value->body && func_value->body->ret)
    {
        expr_check_undefined_ids(tab, func_value->body->ret);
    }
    
    return 0;
}

int func_list_check_undefined_ids(symtab * tab, func_list * list)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {   
        func * func_value = node->value;
        if (func_value)
        {
            func_check_undefined_ids(func_value->stab, func_value);
        }    
        node = node->next;
    }

    return 0;
}

int never_check_undefined_ids(never * nev)
{
    func_list_check_undefined_ids(nev->stab, nev->funcs);

    return 0;
}

int symtab_add_arg_from_arg_list(symtab * tab, arg_list * list)
{
    arg_list_node * node = list->tail;
    while (node != NULL)
    {
        if (node->value)
        {
            symtab_add_arg(tab, node->value);
        }
        node = node->next;
    }
    return 0;
}

int symtab_add_func_from_func_list(symtab * tab, func_list * list)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        if (node->value)
        {
            symtab_add_func(tab, node->value);
        }
        node = node->next;
    }
    return 0;
}

int symtab_add_entry_expr(symtab * stab, expr * value)
{
    switch (value->type)
    {
        case EXPR_INT:
        break;
        case EXPR_ID:
        break;
        case EXPR_NEG:
            symtab_add_entry_expr(stab, value->left);
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
            symtab_add_entry_expr(stab, value->left);
            symtab_add_entry_expr(stab, value->right);
        break;
        case EXPR_SUP:
            symtab_add_entry_expr(stab, value->left);
        break;
        case EXPR_COND:
            symtab_add_entry_expr(stab, value->left);
            symtab_add_entry_expr(stab, value->middle);
            symtab_add_entry_expr(stab, value->right);
        break;
        case EXPR_CALL:
            /* not possible to find function definition here */
        break;
        case EXPR_FUNC:
            symtab_add_entry_func(stab, value->func_value);
        break;
    }
    return 0;
}

int symtab_add_entry_func(symtab * stab_parent, func * func_value)
{
    if (func_value->stab == NULL)
    {
        func_value->stab = symtab_new(32, stab_parent);
    }
    if (func_value->args)
    {
        symtab_add_arg_from_arg_list(func_value->stab, func_value->args);
    }
    if (func_value->body && func_value->body->funcs)
    {
        symtab_add_func_from_func_list(func_value->stab, func_value->body->funcs);
    }
    if (func_value->body && func_value->body->funcs)
    {
        symtab_add_entry_func_list(func_value->stab, func_value->body->funcs);
    }
    if (func_value->body && func_value->body->ret)
    {
        symtab_add_entry_expr(func_value->stab, func_value->body->ret);
    }

    return 0;
}

int symtab_add_entry_func_list(symtab * stab_parent, func_list * list)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value)
        {
            symtab_add_entry_func(stab_parent, func_value);
        }   
        node = node->next;
    }

    return 0;
}

int symtab_add_entry_never(never * nev)
{
    if (nev->stab == NULL)
    {
        nev->stab = symtab_new(32, NULL);
    }
    
    symtab_add_func_from_func_list(nev->stab, nev->funcs);
    symtab_add_entry_func_list(nev->stab, nev->funcs);
    
    return 0;
}

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



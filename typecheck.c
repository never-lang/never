#include <stdio.h>
#include "typecheck.h"
#include "symtab.h"

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

int symtab_add_entry_func_list(func_list * list, symtab * stab_parent)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value)
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
                symtab_add_entry_func_list(func_value->body->funcs, func_value->stab);
            }
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
    
    symtab_add_entry_func_list(nev->funcs, nev->stab);
    
    return 0;
}

int print_symtabs_func_list(func_list * list)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value && func_value->id)
        {
            printf("function: %s\n", func_value->id);
        }
        if (func_value && func_value->stab)
        {
            symtab_print(func_value->stab);
        }
        if (func_value && func_value->body && func_value->body->funcs)
        {
            print_symtabs_func_list(func_value->body->funcs);
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



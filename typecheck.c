#include <stdio.h>
#include "typecheck.h"

int print_func_list(func_list * funcs, int depth)
{
    func_list_node * node = funcs->tail;
    while (node != NULL)
    {
        if (node->value && node->value->id)
        {
            printf("function (%d): %s\n", depth, node->value->id);
        }
        if (node->value && node->value->body && node->value->body->funcs)
        {
            print_func_list(node->value->body->funcs, depth + 1);
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



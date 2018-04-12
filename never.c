#include <stdlib.h>
#include "never.h"

never * never_new(func_list * funcs)
{
    never * n = (never *)malloc(sizeof(never));
    
    n->funcs = funcs;
    
    return n;
}

void never_delete(never * n)
{
    if (n->funcs)
    {
        func_list_delete(n->funcs);
    }
    free(n);
}




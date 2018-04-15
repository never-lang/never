#include <stdlib.h>
#include "never.h"

never * never_new(func_list * funcs)
{
    never * n = (never *)malloc(sizeof(never));
    
    n->funcs = funcs;
    n->stab = NULL;
    
    return n;
}

void never_delete(never * nev)
{
    if (nev->funcs)
    {
        func_list_delete(nev->funcs);
    }
    if (nev->stab)
    {
        symtab_delete(nev->stab);
    }
    free(nev);
}




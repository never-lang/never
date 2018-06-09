#include "optimize.h"
#include "constred.h"

int never_optimize(never * nev)
{
    int ret;

    ret = never_constred(nev);
    
    return ret;
}



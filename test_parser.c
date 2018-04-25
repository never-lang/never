#include <stdio.h>
#include "scanner.h"
#include "parser.h"
#include "never.h"
#include "typecheck.h"
#include "gencode.h"

extern FILE * yyin;

int main(int argc, char * argv[])
{
    int ret;
    never * nev = NULL;
    freopen(argv[1], "r", stdin);

    yyin = stdin;

    ret = yyparse(&nev);
    if (ret == 0)
    {
        ret = never_sem_check(nev);
        if (ret == 0)
        {
            never_gencode(nev);
        }
    }
    
    
    if (nev != NULL)
    {
        never_delete(nev);
    }

    yylex_destroy();

    return 0;
}


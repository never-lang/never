#include <stdio.h>
#include "scanner.h"
#include "parser.h"
#include "never.h"
#include "typecheck.h"

extern FILE * yyin;

int main(int argc, char * argv[])
{
    never * nev = NULL;
    freopen(argv[1], "r", stdin);

    yyin = stdin;

    yyparse(&nev);
    
    never_sem_check(nev);    
    
    yylex_destroy();

    return 0;
}


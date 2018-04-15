#include <stdio.h>
#include "scanner.h"
#include "parser.h"
#include "never.h"

extern FILE * yyin;

int main(int argc, char * argv[])
{
    freopen(argv[1], "r", stdin);

    yyin = stdin;

    yyparse();
    yylex_destroy();

    return 0;
}


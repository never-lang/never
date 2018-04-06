#include <stdio.h>
#include "parser.h"

extern FILE * yyin;

int main(int argc, char * argv[])
{
    yyin = stdin;
    yyparse();

    return 0;
}


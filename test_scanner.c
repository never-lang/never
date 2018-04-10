#include <stdio.h>
#include "types.h"
#include "scanner.h"

extern FILE * yyin;

int main(int argc, char * argv[])
{
    token tokp;

    argv++;
    argc--;

    if (argc > 0)
    {
        yyin = fopen(argv[0], "r");
    }
    else
    {
        yyin = stdin;
    }

    lex_scan(&tokp);

    return 0;
}


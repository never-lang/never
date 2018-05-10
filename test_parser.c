#include <stdio.h>
#include "scanner.h"
#include "parser.h"
#include "never.h"
#include "typecheck.h"
#include "gencode.h"
#include "bytecode.h"

extern FILE * yyin;
extern int parse_result;

int main(int argc, char * argv[])
{
    int ret = 0;
    never * nev = NULL;
    
    if (argc < 2)
    {
        printf("%s: no input files\n", argv[0]);
        return 1;
    }
    
    yyin = fopen(argv[1], "r");
    parse_result = 0;

    yyparse(&nev);
    if (parse_result == 0)
    {
        ret = never_sem_check(nev);
        if (ret == 0)
        {
            bytecode_list * code;
        
            never_gencode(nev);
            
            print_functions(nev);
            
            code = bytecode_new();
            
            never_emit(nev, code);
            
            bytecode_func_addr(code);
            bytecode_print(code);
            bytecode_delete(code);
        }
    }
    
    if (nev != NULL)
    {
        never_delete(nev);
    }

    fclose(yyin);
    yylex_destroy();

    return 0;
}


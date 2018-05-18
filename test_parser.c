#include <stdio.h>
#include "scanner.h"
#include "parser.h"
#include "never.h"
#include "typecheck.h"
#include "gencode.h"
#include "bytecode.h"
#include "vm.h"

extern FILE * yyin;
extern int parse_result;

int execute(bytecode * code_arr, unsigned int code_size)
{
    int ret = 0;
    vm * machine = NULL;

    if (code_arr == NULL)
    {
        return 0;
    }
        
    /* bytecode_array_print(code_arr, code_size); */

    machine = vm_new(1000, 100);
        
    ret = vm_execute(machine, code_arr, code_size);
    printf("result is %d\n", ret);

    vm_delete(machine);
    bytecode_array_delete(code_arr);

    return ret;
}

int main(int argc, char * argv[])
{
    never * nev = NULL;
    unsigned int code_size = 0;
    bytecode * code_arr = NULL;
    
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
        int ret = never_sem_check(nev);
        if (ret == 0)
        {
            bytecode_list * code;
        
            never_gencode(nev);
            
            /* print_functions(nev); */
            
            code = bytecode_new();
            
            never_emit(nev, code);
            
            bytecode_func_addr(code);
            /* bytecode_print(code); */
            
            bytecode_to_array(code, &code_arr, &code_size);
            
            bytecode_delete(code);
        }
    }
    
    if (nev != NULL)
    {
        never_delete(nev);
    }

    fclose(yyin);
    yylex_destroy();

    execute(code_arr, code_size);

    return 0;
}


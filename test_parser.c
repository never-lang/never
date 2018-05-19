#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "scanner.h"
#include "parser.h"
#include "never.h"
#include "typecheck.h"
#include "gencode.h"
#include "bytecode.h"
#include "vm.h"

#define VM_MEM_SIZE 5000
#define VM_STACK_SIZE 200

extern FILE * yyin;
extern int parse_result;
char * utils_file_name;

int execute(bytecode * code_arr, unsigned int code_size)
{
    int res = 0;
    float ret = 0;
    vm * machine = NULL;

    if (code_arr == NULL)
    {
        return 0;
    }
        
    /* bytecode_array_print(code_arr, code_size); */

    machine = vm_new(VM_MEM_SIZE, VM_STACK_SIZE);
        
    res = vm_execute(machine, code_arr, code_size, &ret);
    if (res == 0)
    {
        printf("result is %.2f\n", ret);
    }

    vm_delete(machine);
    bytecode_array_delete(code_arr);

    return 0;
}

int parse_and_exec(char * file_name)
{
    never * nev = NULL;
    unsigned int code_size = 0;
    bytecode * code_arr = NULL;

    utils_file_name = file_name;

    yyin = fopen(file_name, "r");
    if (yyin == NULL)
    {
        fprintf(stderr, "cannot open %s. %s\n", file_name, strerror(errno));
        exit(1); 
    }
    
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

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        printf("%s: no input files\n", argv[0]);
        return 1;
    }
    
    parse_and_exec(argv[1]);

    return 0;
}

